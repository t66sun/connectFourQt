#include "vlc_camera.h"
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    if (start_vlc(0, "/dev/video0") < 0) perror("video0");
    if (start_vlc(1, "/dev/video1") < 0) perror("video1");

    sleep(10);
    stop_all_vlc();

    sleep(2);
    if (start_vlc(0, "/dev/video0") < 0) perror("restart video0");
    if (start_vlc(1, "/dev/video1") < 0) perror("restart video1");

    sleep(10);
    stop_all_vlc();
    return 0;
}
