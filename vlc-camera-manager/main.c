#define _POSIX_C_SOURCE 200809L
#include "vlc_camera.h"
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    VlcManager manager;
    vlc_manager_init(&manager);

    if (start_vlc(&manager, 0, "video0") < 0) perror("start video0");
    if (start_vlc(&manager, 1, "video1") < 0) perror("start video1");
    sleep(10);

    if (stop_all_vlc(&manager) < 0) perror("stop all");
    sleep(2);

    if (start_vlc(&manager, 0, "video0") < 0) perror("restart video0");
    if (start_vlc(&manager, 1, "video1") < 0) perror("restart video1");
    sleep(10);

    if (stop_all_vlc(&manager) < 0) perror("final stop");
    return 0;
}
