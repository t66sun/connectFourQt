#define _POSIX_C_SOURCE 200809L
#include "vlc_camera.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t start_vlc(const char *device)
{
    if (!device || strncmp(device, "/dev/video", 10) != 0 ||
        device[10] == '\0') {
        errno = EINVAL;
        return -1;
    }
    for (const char *p = device + 10; *p; ++p) {
        if (*p < '0' || *p > '9') {
            errno = EINVAL;
            return -1;
        }
    }

    char url[256];
    int n = snprintf(url, sizeof(url), "v4l2://%s", device);
    if (n < 0 || (size_t)n >= sizeof(url)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        execlp("vlc", "vlc", "--no-one-instance", url,
               "--v4l2-width=1920", "--v4l2-height=1080",
               "--v4l2-fps=30", "--live-caching=100",
               (char *)NULL);
        perror("execlp vlc");
        _exit(127);
    }
    return pid;
}

int stop_vlc(pid_t pid)
{
    if (pid <= 0) {
        errno = EINVAL;
        return -1;
    }
    if (kill(pid, SIGTERM) < 0 && errno != ESRCH)
        return -1;
    while (waitpid(pid, NULL, 0) < 0) {
        if (errno == EINTR) continue;
        if (errno == ECHILD) return 0;
        return -1;
    }
    return 0;
}
