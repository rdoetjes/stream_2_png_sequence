# stream_2_png_sequence

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
