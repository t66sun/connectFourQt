#define _POSIX_C_SOURCE 200809L
#include "vlc_camera.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t vlc_pids[MAX_CAMERAS] = {0};

int start_vlc(int index, const char *device)
{
    if (index < 0 || index >= MAX_CAMERAS ||
        !device || strncmp(device, "/dev/video", 10) != 0) {
        errno = EINVAL;
        return -1;
    }

    if (vlc_pids[index] > 0) {
        pid_t result = waitpid(vlc_pids[index], NULL, WNOHANG);
        if (result == 0) { errno = EALREADY; return -1; }
        if (result < 0 && errno != ECHILD) return -1;
        vlc_pids[index] = 0;
    }

    char url[256];
    int n = snprintf(url, sizeof(url), "v4l2://%s", device);
    if (n < 0 || (size_t)n >= sizeof(url)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) return -1;
    if (pid == 0) {
        execlp("vlc", "vlc", "--no-one-instance", url,
               "--live-caching=100", (char *)NULL);
        perror("vlc");
        _exit(127);
    }
    vlc_pids[index] = pid;
    return 0;
}

int stop_vlc(int index)
{
    if (index < 0 || index >= MAX_CAMERAS) {
        errno = EINVAL;
        return -1;
    }
    pid_t pid = vlc_pids[index];
    if (pid <= 0) return 0;

    if (kill(pid, SIGTERM) < 0 && errno != ESRCH) return -1;
    while (waitpid(pid, NULL, 0) < 0) {
        if (errno == EINTR) continue;
        if (errno != ECHILD) return -1;
        break;
    }
    vlc_pids[index] = 0;
    return 0;
}

void stop_all_vlc(void)
{
    for (int i = 0; i < MAX_CAMERAS; i++)
        if (stop_vlc(i) < 0) perror("stop_vlc");
}
