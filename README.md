# AtlanticBeast

Work in progress. In demo we can play a simple minecraft look a like game where you can place blocks.

Current default demo keybinds:

W - Move forward<br>
S - Move backward<br>
D - Strafe right<br>
A - Strafe left<br>
E - Fly up<br>
Q - Fly down<br>
<br>
Mouse - camera rotation<br>
<br>
1 - Rotate yaw to the right<br>
2 - Rotate yaw to the left<br>
3 - Pitch up<br>
4 - Pitch down<br>
5 - Increase fov<br>
6 - Decrease fov<br>
<br>
SPACE - Place a block<br>
<br>
<br>



<img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot00.jpg?raw=true" alt="Screenshot" width="300"/>

![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot00.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot01.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot02.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot03.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot04.jpg?raw=true)

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
cmake --build .
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
cmake --build .
```
