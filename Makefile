CC = gcc
CFLAGS = -Wall -Wextra -g
GTK_FLAGS = `pkg-config --cflags --libs gtk+-3.0`

all: gui 

gui: gui.c video.h
	$(CC) $(CFLAGS) -o gui gui.c $(GTK_FLAGS)

clean:
	rm -f gui video