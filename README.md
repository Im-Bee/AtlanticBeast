# AtlanticBeast

Work in progress.

Current default demo keybinds:

W - Move forward along Z axis<br>
S - Move backward along Z axis<br>
D - Move forward (right) along X axis<br>
A - Move backward (left) along X axis<br>
<br>
1 - Rotate yaw to the right<br>
2 - Rotate yaw to the left<br>
3 - Pitch up<br>
4 - Pitch down<br>
5 - Increase fov<br>
6 - Decrease fov<br>





![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/ReadMeScreenshot.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/ReadMeScreenshot2.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot01.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot02.jpg?raw=true)

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
