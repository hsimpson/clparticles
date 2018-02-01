# CLParticles

Calculating particles with OpenCL and rendering with OpenGL

## Dependencies

- gl3w
- glfw

## Building

### Prerequisites

- Visual Studio 2017
- Python
- CMake

### Build gl3w

```sh
cd gl3w
python gl3w_gen.py
```

### Build glfw

```sh
cd glfw
mkdir glfw-build
cd glfw-build
cmake .. -DBUILD_SHARED_LIBS=ON -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -G "Visual Studio 15 2017 Win64"
```

Open ```glfw-build\GLFW.sln``` in Visual Studio 2017 and build both Debug and Release