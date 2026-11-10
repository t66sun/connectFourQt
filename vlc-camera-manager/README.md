# Simple VLC multi-camera controller

Linux C example: start several independent VLC processes, stop them, and reopen the same devices.

## Build

```sh
sudo apt install vlc build-essential
make
./vlc-camera-demo
```

## Use

```c
start_vlc(0, "/dev/video0");
start_vlc(1, "/dev/video1");
stop_vlc(0);
start_vlc(0, "/dev/video0");
stop_all_vlc();
```

Each index from 0 to 7 holds one VLC process. Device arguments are **full Linux device paths** such as `/dev/video0`. The two processes can run concurrently.

The child runs `vlc --no-one-instance v4l2:///dev/video0 --live-caching=100` (or the corresponding device).

**Notes:** The start function confirms that `fork()` succeeded, but does not confirm that VLC opened the camera or even executed successfully. `stop_vlc()` waits until VLC exits, and can block if VLC hangs. Use this simple version from one thread; do not manually reap its child processes. It does not stop unrelated VLC instances. Verify available devices with `v4l2-ctl --list-devices`.
