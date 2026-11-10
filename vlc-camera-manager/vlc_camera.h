#ifndef VLC_CAMERA_H
#define VLC_CAMERA_H

/* index: 0..7; device: e.g. "/dev/video0" */
int start_vlc(int index, const char *device);
int stop_vlc(int index);
void stop_all_vlc(void);

#endif
