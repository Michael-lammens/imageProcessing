CC = gcc
CFLAGS = -Wall -Wextra -g
GTK_FLAGS = `pkg-config --cflags --libs gtk+-3.0`

all: gui video

gui: gui.c
	$(CC) $(CFLAGS) -o gui gui.c $(GTK_FLAGS)

video: video.c
	$(CC) $(CFLAGS) -o video video.c

clean:
	rm -f gui video