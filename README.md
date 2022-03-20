# stream_2_png_sequence

Will take a video stream, and convert it to an ASCII art 1920x1080 PNG sequence. This way you can convert any movie into ASCII art for a nice visual effect.

[![IMAGE ALT TEXT](http://img.youtube.com/vi/5H2aLGLSEv4/0.jpg)]

# Build on MAC
When build on mac using a homebrew OpenCV version, requires you to use clang and not homebrew gcc

export CXX=/usr/bin/clang++
export CC=/usr/bin/clang

# Build instruction
Edit the CMakeFiles.txt to point to your OpenCV directories

```
cd stream_2_png_sequence
mkdir build
cd build
cmake ..
make

copy the ascii binary and the ibm.ttf, whereever you want it to live. The ibm.ttf font is expected to live in the same directory.
```

# Running

For using the webcam
```
./ascii movie.mp4 render_dir/
```
