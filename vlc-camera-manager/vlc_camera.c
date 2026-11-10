#define _POSIX_C_SOURCE 200809L
#include "vlc_camera.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void vlc_manager_init(VlcManager *manager)
{
    if (manager) memset(manager, 0, sizeof(*manager));
}

static int valid_device(const char *name)
{
    if (!name || strncmp(name, "video", 5) != 0 || name[5] == '\0')
        return 0;
    for (const char *p = name + 5; *p; ++p)
        if (*p < '0' || *p > '9') return 0;
    return 1;
}

static int reap_if_exited(VlcCamera *cam)
{
    if (cam->pid <= 0) return 1;
    int status;
    pid_t result;
    do { result = waitpid(cam->pid, &status, WNOHANG); }
    while (result < 0 && errno == EINTR);
    if (result == cam->pid || (result < 0 && errno == ECHILD)) {
        cam->pid = 0;
        return 1;
    }
    return result < 0 ? -1 : 0;
}

int start_vlc(VlcManager *manager, size_t index, const char *device)
{
    if (!manager || index >= VLC_MAX_CAMERAS || !valid_device(device)) {
        errno = EINVAL;
        return -1;
    }
    VlcCamera *cam = &manager->cameras[index];
    int state = reap_if_exited(cam);
    if (state < 0) return -1;
    if (state == 0) { errno = EALREADY; return -1; }

    char url[128];
    int n = snprintf(url, sizeof(url), "v4l2:///dev/%s", device);
    if (n < 0 || (size_t)n >= sizeof(url)) { errno = ENAMETOOLONG; return -1; }

    /* Close-on-exec pipe lets the parent detect exec failure. */
    int pipefd[2];
    if (pipe(pipefd) < 0) return -1;
    if (fcntl(pipefd[1], F_SETFD, FD_CLOEXEC) < 0) {
        int saved = errno;
        close(pipefd[0]); close(pipefd[1]);
        errno = saved; return -1;
    }
    pid_t pid = fork();
    if (pid < 0) {
        int saved = errno;
        close(pipefd[0]); close(pipefd[1]);
        errno = saved; return -1;
    }
    if (pid == 0) {
        close(pipefd[0]);
        execlp("vlc", "vlc", "--no-one-instance", url,
               "--v4l2-width=1920", "--v4l2-height=1080",
               "--v4l2-fps=30", "--live-caching=100", (char *)NULL);
        int saved = errno;
        (void)write(pipefd[1], &saved, sizeof(saved));
        _exit(127);
    }
    close(pipefd[1]);
    int exec_errno = 0;
    ssize_t count;
    do { count = read(pipefd[0], &exec_errno, sizeof(exec_errno)); }
    while (count < 0 && errno == EINTR);
    int read_errno = errno;
    close(pipefd[0]);
    if (count != 0) {
        int status;
        if (count > 0) {
            while (waitpid(pid, &status, 0) < 0 && errno == EINTR) {}
            errno = exec_errno;
        } else {
            (void)kill(pid, SIGKILL);
            while (waitpid(pid, &status, 0) < 0 && errno == EINTR) {}
            errno = read_errno;
        }
        return -1;
    }
    cam->pid = pid;
    snprintf(cam->device, sizeof(cam->device), "%s", device);
    return 0;
}

int stop_vlc(VlcManager *manager, size_t index)
{
    if (!manager || index >= VLC_MAX_CAMERAS) { errno = EINVAL; return -1; }
    VlcCamera *cam = &manager->cameras[index];
    int state = reap_if_exited(cam);
    if (state < 0) return -1;
    if (state == 1) return 0;

    if (kill(cam->pid, SIGTERM) < 0 && errno != ESRCH) return -1;

    /* Wait up to about 2 seconds before forcefully terminating. */
    const struct timespec delay = { .tv_sec = 0, .tv_nsec = 100000000L };
    for (int i = 0; i < 20; ++i) {
        state = reap_if_exited(cam);
        if (state < 0) return -1;
        if (state == 1) return 0;
        nanosleep(&delay, NULL);
    }
    if (kill(cam->pid, SIGKILL) < 0 && errno != ESRCH) return -1;
    int status;
    pid_t result;
    do { result = waitpid(cam->pid, &status, 0); }
    while (result < 0 && errno == EINTR);
    if (result < 0 && errno != ECHILD) return -1;
    cam->pid = 0;
    return 0;
}

int stop_all_vlc(VlcManager *manager)
{
    if (!manager) { errno = EINVAL; return -1; }
    int failed = 0;
    for (size_t i = 0; i < VLC_MAX_CAMERAS; ++i)
        if (stop_vlc(manager, i) < 0) failed = 1;
    return failed ? -1 : 0;
}
