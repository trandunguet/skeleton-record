Record skeleton movement and save to disk.
Device: Kinect v2

# OpenNI2, OpenCV
`$ sudo apt install libopenni2-dev libopencv-dev`

# Kinect driver
https://github.com/OpenKinect/libfreenect2

# NiTE 
https://bitbucket.org/kaorun55/openni-2.2/src/2f54272802bf/NITE%202.2%20%CE%B1/?at=master

# Build & run
```
$ mkdir build
$ cd build
$ cp ~/NiTE-Linux-x64-2.2/Redist/NiTE2 .
$ cmake ..
$ make
$ ./Record
```

Step in camera's FOV to start recording, step out or press ESC to stop.
