# stream_2_png_sequence

Will take a video stream, and convert it to an ASCII art 1920x1080 PNG sequence. This way you can convert any movie into ASCII art for a nice visual effect.

[![IMAGE ALT TEXT](http://img.youtube.com/vi/5H2aLGLSEv4/0.jpg)]

Build on mac with homebrew OpenCV requires clang

export CXX=/usr/bin/clang++
export CC=/usr/bin/clang

# Build instruction
Edit the CMakeFiles.txt to point to your OpenCV directorys

```
cd stream_2_png_sequence
mkdir build
cd build
cmake ..
make
```

# Running

For using the webcam
```
./ascii movie.mp4 render_dir/
```
