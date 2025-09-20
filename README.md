# AtlanticBeast

Work in progress.

![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/ReadMeScreenshot.jpg?raw=true)
![alt text](https://github.com/Im-Bee/AtlanticBeast/blob/master/Docs/ReadMeScreenshot2.jpg?raw=true)

# Build

Build dependencies:
  - Vulkan SDK
  - CMake or Visual Studio with CMake support
  - Compiler that supports at least C++17
  - Windows or Linux operating system

## Linux

!!! In current version after building the project, you have to CD in to directory with executable, because VoxelsLib looks for shaders in current directory.

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

