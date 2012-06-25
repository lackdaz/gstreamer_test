#include <string.h>
#include <gst/gst.h>
#include <gst/interfaces/colorbalance.h>
  
typedef struct _CustomData {
  GstElement *pipeline;
  GMainLoop *loop;
} CustomData;
  
/* playbin2 flags */
typedef enum {
  GST_PLAY_FLAG_DEINTERLACE   = (1 << 9)
} GstPlayFlags;
  
/* Process a color balance command */
static void update_color_channel (const gchar *channel_name, gboolean increase, GstColorBalance *cb) {
  gdouble step;
  gint value;
  GstColorBalanceChannel *channel = NULL;
  const GList *channels, *l;
  
  /* Retrieve the list of channels and locate the requested one */
  channels = gst_color_balance_list_channels (cb);
  for (l = channels; l != NULL; l = l->next) {
    GstColorBalanceChannel *tmp = (GstColorBalanceChannel *)l->data;
    
    if (g_strrstr (tmp->label, channel_name)) {
      channel = tmp;
      break;
    }
  }
  if (!channel)
    return;
  
  /* Change the channel's value */
  step = 0.1 * (channel->max_value - channel->min_value);
  value = gst_color_balance_get_value (cb, channel);
  if (increase) {
    value += step;
    if (value > channel->max_value)
        value = channel->max_value;
  } else {
    value -= step;
    if (value < channel->min_value)
        value = channel->min_value;
  }
  gst_color_balance_set_value (cb, channel, value);
}
  
/* Toggle the deinterlacing flag */
static void toggle_deinterlace (GstElement *pipeline) {
  gint flags;
  gint64 current_position = GST_CLOCK_TIME_NONE;
  GstFormat format = GST_FORMAT_TIME;
  
  /* Find current position, since it will be lost when we stop */
  gst_element_query_position (pipeline, &format, &current_position);
  /* Stop */
  gst_element_set_state (pipeline, GST_STATE_READY);
  /* Toggle deinterlacing flag (it will be ignored while PLAYING) */
  g_object_get (pipeline, "flags", &flags, NULL);
  flags ^= GST_PLAY_FLAG_DEINTERLACE;
  g_object_set (pipeline, "flags", flags, NULL);
  /* Resume */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  /* Wait until the state change takes effect */
  gst_element_get_state (pipeline, NULL, NULL, GST_CLOCK_TIME_NONE);
  /* Set current position, if we were able to recover it previously */
  if (GST_CLOCK_TIME_IS_VALID (current_position)) {
    gst_element_seek_simple (pipeline, format, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE, current_position);
  }
}
  
/* Output the current values of all Color Balance channels and the Deinterlace flag */
static void print_current_values (GstElement *pipeline) {
  gint flags;
  const GList *channels, *l;
  
  /* Output Color Balance values */
  channels = gst_color_balance_list_channels (GST_COLOR_BALANCE (pipeline));
  for (l = channels; l != NULL; l = l->next) {
    GstColorBalanceChannel *channel = (GstColorBalanceChannel *)l->data;
    gint value = gst_color_balance_get_value (GST_COLOR_BALANCE (pipeline), channel);
    g_print ("%s: %3d%% ", channel->label,
        100 * (value - channel->min_value) / (channel->max_value - channel->min_value));
  }
  
  /* Output Deinterlace flag value */
  g_object_get (pipeline, "flags", &flags, NULL);
  g_print ("Deinterlacing: %s\n", flags & GST_PLAY_FLAG_DEINTERLACE ? "ON" : "OFF");
}
  
/* Process keyboard input */
static gboolean handle_keyboard (GIOChannel *source, GIOCondition cond, CustomData *data) {
  gchar *str = NULL;
  
  if (!g_io_channel_read_line (source, &str, NULL, NULL, NULL) == G_IO_STATUS_NORMAL) {
    return TRUE;
  }
  
  switch (g_ascii_tolower (str[0])) {
  case 'c':
    update_color_channel ("CONTRAST", g_ascii_isupper (str[0]), GST_COLOR_BALANCE (data->pipeline));
    break;
  case 'b':
    update_color_channel ("BRIGHTNESS", g_ascii_isupper (str[0]), GST_COLOR_BALANCE (data->pipeline));
    break;
  case 'h':
    update_color_channel ("HUE", g_ascii_isupper (str[0]), GST_COLOR_BALANCE (data->pipeline));
    break;
  case 's':
    update_color_channel ("SATURATION", g_ascii_isupper (str[0]), GST_COLOR_BALANCE (data->pipeline));
    break;
  case 'd':
    toggle_deinterlace (data->pipeline);
    break;
  case 'q':
    g_main_loop_quit (data->loop);
    break;
  default:
    break;
  }
  
  g_free (str);
  
  print_current_values (data->pipeline);
  
  return TRUE;
}
  
int main(int argc, char *argv[]) {
  CustomData data;
  GstStateChangeReturn ret;
  GIOChannel *io_stdin;
   
  /* Initialize GStreamer */
  gst_init (&argc, &argv);
  
  /* Initialize our data structure */
  memset (&data, 0, sizeof (data));
  
  /* Print usage map */
  g_print (
    "USAGE: Choose one of the following options, then press enter:\n"
    " 'C' to increase contrast, 'c' to decrease contrast\n"
    " 'B' to increase brightness, 'b' to decrease brightness\n"
    " 'H' to increase hue, 'h' to decrease hue\n"
    " 'S' to increase saturation, 's' to decrease saturation\n"
    " 'D' to toggle deinterlacing ON and OFF\n"
    " 'Q' to quit\n");
  
  /* Build the pipeline */
  data.pipeline = gst_parse_launch ("playbin2 uri=http://docs.gstreamer.com/media/sintel_trailer-480i.avi", NULL);
  
  /* Add a keyboard watch so we get notified of keystrokes */
#ifdef _WIN32
  io_stdin = g_io_channel_win32_new_fd (fileno (stdin));
#else
  io_stdin = g_io_channel_unix_new (fileno (stdin));
#endif
  g_io_add_watch (io_stdin, G_IO_IN, (GIOFunc)handle_keyboard, &data);
  
  /* Start playing */
  ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (data.pipeline);
    return -1;
  }
  print_current_values (data.pipeline);
   
  /* Create a GLib Main Loop and set it to run */
  data.loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (data.loop);
  
  /* Free resources */
  g_main_loop_unref (data.loop);
  g_io_channel_unref (io_stdin);
  gst_element_set_state (data.pipeline, GST_STATE_NULL);
  gst_object_unref (data.pipeline);
  return 0;
}
