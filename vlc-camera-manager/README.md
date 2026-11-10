# VLC multi-camera manager

Simple Linux C functions for running several independent VLC instances with V4L2 cameras.

```c
start_vlc(0, "/dev/video0");
start_vlc(1, "/dev/video1");
stop_vlc(0);
start_vlc(0, "/dev/video0");
stop_all_vlc();
```

- Index range: 0–7. Each index controls one VLC child process.
- Device argument: full path such as `/dev/video0`.
- No initialization needed.
- Requires VLC on PATH and accessible V4L2 devices.
- Compile: `make`; run: `./vlc-camera-demo`.

This intentionally minimal example uses `fork`, `execlp`, `SIGTERM`, and `waitpid`. It does not verify whether VLC successfully opened the camera; it only reports fork errors immediately. `stop_vlc` waits for VLC to exit and may block if VLC hangs. Intended for single-threaded use; do not concurrently reap these child PIDs elsewhere. The repository is public.
