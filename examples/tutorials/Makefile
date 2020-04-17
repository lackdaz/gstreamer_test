# LIBS  = -lkernel32 -luser32 -lgdi32 -lopengl32 
CFLAGS = -Wall `pkg-config --cflags --libs gstreamer-1.0 gstreamer-audio-1.0 gstreamer-video-1.0 gtk+-3.0`
CC = gcc
.PHONY: all clean
.SECONDARY: main-build

OUT_PATH = '/bin'
OUT_DIR = $(CURDIR)$(OUT_PATH)
# Should be equivalent to your list of C files, if you don't build selectively
SRCS=$(wildcard *.c)
EXECS=$(wildcard $(CURDIR)$(OUT_DIR)/*.out)

PROGS = $(patsubst %.c,%,$(SRCS))

all: ${OUT_DIR} main-build
	@printf "\033[32;1mBuild Complete!\033[0m\n"

main-build: $(PROGS)

%: %.c
	$(CC) -o $(OUT_DIR)/$@.out $< $(CFLAGS)

clean:
	@printf "\033[41;1mCleaning up...\033[0m\n"
	@rm -rf $(OUT_DIR)/*.out 
	@printf "\033[32;1mClean Complete!\033[0m\n"

test:
	@echo $(OUT_DIR)/*.out

${OUT_DIR}:
	@printf "\033[41;1mCreating ${OUT_PATH} folder...\033[0m\n"
	@mkdir -p $(OUT_DIR)