# AtlanticBeast

Heavy work in progress.

![alt text](https://github.com/[username]/[reponame]/blob/[branch]/Docs/ReadMeScreenshot.png?raw=true)

# Build

## Needs Vulkan SDK to compile.

### Linux

In current version after building the project, you have to CD in to directory with executable, because VoxelsLib looks for shaders in current directory.

``` sh
mkdir Build
cd Build
cmake ..
make 
```

### Windows

### Visual Studio 

Open root directory of the project with Visual Studio. Wait for cmake to build. Select root CMakeLists.txt as a build target. 

