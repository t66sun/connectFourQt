# VLC multi-camera manager (Linux)

Small C wrapper for starting, stopping, and restarting independent VLC processes that capture V4L2 devices such as `/dev/video0`.

## Requirements

- Linux with V4L2 camera devices
- VLC installed and available as `vlc` on PATH
- A C11 compiler and make

Install on Ubuntu: `sudo apt install vlc v4l-utils build-essential`.

## Build and run

```sh
make
./vlc-camera-demo
```

The demo starts `video0` and `video1`, stops them after 10 seconds, restarts them, and stops them again. Edit `main.c` for your devices.

## API

```c
VlcManager manager;
vlc_manager_init(&manager);
start_vlc(&manager, 0, "video0");
start_vlc(&manager, 1, "video1");
stop_vlc(&manager, 0);
start_vlc(&manager, 0, "video0");
stop_all_vlc(&manager);
```

Each slot (0 through `VLC_MAX_CAMERAS - 1`) stores its own child PID. VLC is started with `--no-one-instance`. The module uses a close-on-exec pipe to report exec failures and reaps exited children. Stop sends SIGTERM, waits up to about two seconds, then SIGKILL if needed.

**Limitations:** Success from `start_vlc` means VLC was executed, not that the camera successfully opened. Device availability, supported resolution/frame rate, USB bandwidth, and VLC V4L2 options must be checked separately. The module assumes a single-threaded manager and that no other code reaps its child processes. Do not call `vlc_manager_init` on an active manager; stop all processes first. Stop may block briefly. This example is not a libVLC integration.

Use `v4l2-ctl --list-devices` and `v4l2-ctl -d /dev/video0 --list-formats-ext` to inspect cameras. The example requests 1920x1080 at 30 fps; change the flags in `vlc_camera.c` to suit your devices.
