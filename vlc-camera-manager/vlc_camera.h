#ifndef VLC_CAMERA_H
#define VLC_CAMERA_H

#include <sys/types.h>
#include <stddef.h>

#define VLC_MAX_CAMERAS 8
#define VLC_DEVICE_NAME_MAX 64

typedef struct {
    char device[VLC_DEVICE_NAME_MAX];
    pid_t pid;
} VlcCamera;

typedef struct {
    VlcCamera cameras[VLC_MAX_CAMERAS];
} VlcManager;

/* Initialize before using the manager. */
void vlc_manager_init(VlcManager *manager);
/* device is a basename such as "video0", not a full path.
 * Returns 0 on successful fork/exec, -1 on error (errno set).
 * Successful exec does not guarantee successful device capture. */
int start_vlc(VlcManager *manager, size_t index, const char *device);
/* Stop and reap the managed child. A timeout escalates to SIGKILL. */
int stop_vlc(VlcManager *manager, size_t index);
/* Stop every managed child; returns -1 if any stop fails. */
int stop_all_vlc(VlcManager *manager);

#endif
