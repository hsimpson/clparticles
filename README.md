# CLParticles

Calculating particles with OpenCL and rendering with OpenGL

## Dependencies

- gl3w
- glfw
- glm

## Building

### Prerequisites

- Visual Studio 2017
- Python
- CMake
- Vcpkg

### Build gl3w

```sh
cd gl3w
python gl3w_gen.py
```

### Build Vcpkg

```sh
cd vcpkg
.\bootstrap-vcpkg.bat
```

### Install deps via Vcpkg

```sh
vcpkg.exe install glfw3:x64-windows
vcpkg.exe install glm:x64-windows
```
