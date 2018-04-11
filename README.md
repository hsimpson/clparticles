# CLParticles

Calculating particles with OpenCL 1.2 and rendering with OpenGL 4.6

![Screenshot](media/screenshots/clparticles_2018-04-06_01-24-12.png)

## Dependencies

- gl3w <https://github.com/skaslev/gl3w>
- glfw <http://www.glfw.org/>
- glm <https://glm.g-truc.net>
- imgui <https://github.com/ocornut/imgui>
- vcpkg <https://github.com/Microsoft/vcpkg>

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
