# VLC camera launcher (Linux)

Simple C functions to launch independent VLC processes for V4L2 devices.

## Build

Install VLC and a compiler, then:

```sh
make
./vlc-camera-demo
```

## Usage

```c
pid_t cam0 = start_vlc("/dev/video0");
pid_t cam1 = start_vlc("/dev/video1");
if (cam0 > 0) stop_vlc(cam0);
if (cam1 > 0) stop_vlc(cam1);
cam0 = start_vlc("/dev/video0"); // restart
```

`start_vlc` accepts a full path in the form `/dev/videoN` and returns the spawned PID or -1 on fork/validation error. `stop_vlc` sends SIGTERM and waits for the child to exit. It must be called from the parent process that started VLC. Never pass -1 or unrelated PIDs.

**Notes:** Successful fork does not guarantee that VLC executed or that the camera opened. VLC must be on PATH. Stop may block if VLC fails to exit after SIGTERM. This is a deliberately minimal, single-threaded example. The requested 1920x1080 at 30 fps may need changing to match camera capabilities. Run `v4l2-ctl -d /dev/video0 --list-formats-ext` to check. No existing Qt files are used or modified.
