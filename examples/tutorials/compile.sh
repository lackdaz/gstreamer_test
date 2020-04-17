#!/usr/bin/env bash

for i in *.c
do
    # echo "gcc "$i" -o "${i%.c}" `pkg-config --cflags --libs gstreamer-1.0 gstreamer-audio-1.0`"
    gcc "$i" -o "${i%.c}.out" `pkg-config --cflags --libs gstreamer-1.0 gstreamer-audio-1.0`
done
