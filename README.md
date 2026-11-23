# opengl-renderer

A personal OpenGL rendering project, written in C++20 while learning real-time graphics. It opens a window with GLFW and renders 3D models loaded with Assimp.

All dependencies are downloaded and built automatically by CMake: you only need a C++ compiler, CMake and Git.

## Requirements

- A C++20 compiler (Apple Clang, MSVC, GCC or Clang)
- [CMake](https://cmake.org/download/) 3.20 or newer
- [Git](https://git-scm.com/)
- An internet connection for the first configuration (dependencies are fetched from GitHub)

**Tested on:** macOS (Apple Silicon). Windows and Linux should work but have not been tested yet.

## Build and run

```bash
git clone https://github.com/Antoine-sama/opengl-renderer.git
cd opengl-renderer
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The first build is slow because GLFW, GLM and Assimp are compiled from source. Later builds are fast.

The executable is placed in `build/bin/`:

```bash
./build/bin/OpenGLTraining
```

### macOS

Install the command line tools and CMake:

```bash
xcode-select --install
brew install cmake
```

Then follow the build steps above.

### Windows

Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with the **Desktop development with C++** workload (it includes CMake), then in a Developer PowerShell:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\bin\Release\OpenGLTraining.exe
```

### Linux (Debian / Ubuntu)

GLFW needs the X11 and Wayland development packages:

```bash
sudo apt install build-essential cmake git libgl1-mesa-dev xorg-dev \
    libwayland-dev libxkbcommon-dev wayland-protocols
```

Then follow the build steps above.

## Using an IDE

The project is a standard CMake project, so you can open the folder directly in CLion, Visual Studio, VS Code (with the CMake Tools extension) or Qt Creator. For editors based on clangd, a `compile_commands.json` file is generated in the build folder.

## Project structure

```
.
├── CMakeLists.txt   Build configuration (dependencies are fetched here)
├── src/             Source code
├── assets/          Shaders, textures and 3D models
│   ├── models/
│   ├── shaders/
│   └── textures/
└── external/        Third-party code that is stored in the repository
    ├── glad/        OpenGL loader (generated files)
    └── stb/         stb_image.h
```

## Notes

- **Assets path:** the path to the `assets/` folder is written into the executable at compile time. The program therefore works from any directory, but the repository folder must stay where it was when you built it. If you move the repository, re-run CMake and rebuild.
- **OpenGL on macOS:** Apple deprecated OpenGL and stopped at version 4.1, so only features up to OpenGL 4.1 are available there.

## Dependencies

| Library | Purpose | How it is obtained |
|---|---|---|
| [GLFW](https://www.glfw.org/) 3.4 | Window and input | Fetched by CMake |
| [GLM](https://github.com/g-truc/glm) 1.0.1 | Math library | Fetched by CMake |
| [Assimp](https://github.com/assimp/assimp) 6.0 | 3D model loading (OBJ, FBX, glTF) | Fetched by CMake |
| [glad](https://github.com/Dav1dde/glad) | OpenGL function loader | Included in `external/glad` |
| [stb_image](https://github.com/nothings/stb) | Image loading | Included in `external/stb` |

To load other model formats, enable the matching importer (`ASSIMP_BUILD_<FORMAT>_IMPORTER`) in `CMakeLists.txt`.

## Troubleshooting

- **Shaders or models cannot be read at launch:** the repository folder was probably moved after the build. Re-run CMake and rebuild.
- **CMake cannot find glad:** make sure `external/glad/src/glad.c` exists.
- **GLFW fails to build on Linux:** install the X11 and Wayland development packages listed above.