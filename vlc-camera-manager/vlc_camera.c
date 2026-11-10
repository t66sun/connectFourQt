#define _POSIX_C_SOURCE 200809L
#include "vlc_camera.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CAMERAS 8
static pid_t pids[MAX_CAMERAS];

int start_vlc(int index, const char *device)
{
    if (index < 0 || index >= MAX_CAMERAS || !device ||
        strncmp(device, "/dev/video", 10) != 0) {
        errno = EINVAL;
        return -1;
    }

    if (pids[index] > 0) {
        pid_t result = waitpid(pids[index], NULL, WNOHANG);
        if (result == 0) { errno = EALREADY; return -1; }
        if (result < 0 && errno != ECHILD) return -1;
        pids[index] = 0;
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
        perror("execlp vlc");
        _exit(127);
    }

    pids[index] = pid;
    return 0;
}

int stop_vlc(int index)
{
    if (index < 0 || index >= MAX_CAMERAS) {
        errno = EINVAL;
        return -1;
    }
    if (pids[index] <= 0) return 0;

    pid_t pid = pids[index];
    if (kill(pid, SIGTERM) < 0 && errno != ESRCH) return -1;

    int status;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR) continue;
        if (errno != ECHILD) return -1;
        break;
    }
    pids[index] = 0;
    return 0;
}

void stop_all_vlc(void)
{
    for (int i = 0; i < MAX_CAMERAS; ++i)
        if (stop_vlc(i) < 0) perror("stop_vlc");
}
