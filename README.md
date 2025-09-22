# AtlanticBeast

Work in progress.

Current default demo keybinds:

W - Move forward along Z axis
S - Move backward along Z axis
D - Move forward (right) along X axis
A - Move backward (left) along X axis

1 - Rotate yaw to the right
2 - Rotate yaw to the left
3 - Pitch up
4 - Pitch down





![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/ReadMeScreenshot.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/ReadMeScreenshot2.jpg?raw=true)

# Build

Build dependencies:
  - Vulkan SDK
  - CMake or Visual Studio with CMake support
  - Compiler that supports at least C++17
  - Windows or Linux operating system

## Linux

### CMake

``` sh
mkdir Build &&
cd Build &&
cmake .. -DCMAKE_BUILD_TYPE=Release &&
make 
```

## Windows

### Visual Studio 

Open root directory of the project with Visual Studio. Wait for cmake to build. Select AtlBee project as a build target.
Prefer release build.

### CMake

``` sh
mkdir Build
cd Build
cmake .. -DCMAKE_BUILD_TYPE=Release
make 
```
