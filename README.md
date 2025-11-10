# AtlanticBeast

Realtime GPU ray tracer with custom acceleration structure based on a simplistic voxel grid in vulkan and Minecraft alike demo game. <br>

Project uses voxel traversal algorithm implementation based on <br>
'A Fast Voxel Traversal Algorithm for Ray Tracing'<br>
by John Amanatides and Andrew Woo (1987) and Blinn-Phong Reflection lighting.<br>

Implements a custom cross-platform window management system with runtime switchable behavior.<br>

Features a low-latency input system and a fully custom Vulkan-based compute rendering pipeline.<br>

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

# Gif from tests (bugs featured)
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/2025-11-10%2003-42-03.gif?raw=true" alt="Screenshot" width="45%"/>
</p>

# Screenshots
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen01.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen02.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>
<p float="left">
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen03.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/readmeSrceenshot_6.jpg?raw=true" alt="Screenshot" width="49%"/>
</p>																
<p float="left">													
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen05.jpg?raw=true" alt="Screenshot" width="49%"/>
  <img src="https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/DocScreen06.jpg?raw=true" alt="Screenshot" width="49%"/>
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


