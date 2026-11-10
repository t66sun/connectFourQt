CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: vlc-camera-demo

vlc-camera-demo: main.o vlc_camera.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c vlc_camera.h
vlc_camera.o: vlc_camera.c vlc_camera.h

clean:
	rm -f main.o vlc_camera.o vlc-camera-demo
