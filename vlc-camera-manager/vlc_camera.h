#ifndef VLC_CAMERA_H
#define VLC_CAMERA_H
#include <sys/types.h>

/* Returns child PID, or -1 if fork fails. */
pid_t start_vlc(const char *device);
/* Stop and wait for a VLC child launched by start_vlc. */
int stop_vlc(pid_t pid);

#endif
