#include "vlc_camera.h"
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t cam0 = start_vlc("/dev/video0");
    pid_t cam1 = start_vlc("/dev/video1");
    if (cam0 < 0) perror("start video0");
    if (cam1 < 0) perror("start video1");

    sleep(10);
    if (cam0 > 0 && stop_vlc(cam0) < 0) perror("stop video0");
    if (cam1 > 0 && stop_vlc(cam1) < 0) perror("stop video1");

    cam0 = start_vlc("/dev/video0");
    cam1 = start_vlc("/dev/video1");
    sleep(10);

    if (cam0 > 0 && stop_vlc(cam0) < 0) perror("stop video0");
    if (cam1 > 0 && stop_vlc(cam1) < 0) perror("stop video1");
    return 0;
}
