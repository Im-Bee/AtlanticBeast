# AtlanticBeast

Project uses voxel traversal algorithm implementation based on <br>
'A Fast Voxel Traversal Algorithm for Ray Tracing'<br>
by John Amanatides and Andrew Woo (1987) and Phong Reflection lighting.

# Demo

In demo we can play a simple minecraft lookalike game where you can place and remove blocks.

## Current default demo keybinds:

### Movement:
W - Move forward<br>
S - Move backward<br>
D - Strafe right<br>
A - Strafe left<br>
E - Fly up<br>
Q - Fly down<br>
Mouse - camera rotation<br>

### Interactions:
SPACE - Place a block<br>
X - Remove a block<br>

### Debug:
1 - Rotate yaw to the left<br>
2 - Rotate yaw to the right<br>
3 - Pitch up<br>
4 - Pitch down<br>
5 - Increase fov<br>
6 - Decrease fov<br>

# Screenshots
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot00.jpg?raw=true" alt="Screenshot" width="45%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot01.jpg?raw=true" alt="Screenshot" width="45%"/>
</p>
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot04.jpg?raw=true" alt="Screenshot" width="45%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/screenshot03.jpg?raw=true" alt="Screenshot" width="45%"/>
</p>

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


