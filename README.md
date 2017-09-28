# Computer-Graphics
This a C++ OpenGL repository to contain my work for COMP371 from Concordia Universirty during the Fall 2017 semester

## Modules
- [Pseudo Pac-Man](https://github.com/prince-chrismc/Computer-Graphics/blob/master/PacmanA1)

## System Setup
> Currently the projects are hard coded to my workstations layout due to the CMake provided for the assignement. This needs to be correct with enviroment variables setup so i can migrate my work to my laptop as well

- Visual Studio
- OpenGL Libraries
  - [GLFW 3.2.1](https://github.com/glfw/glfw/releases/tag/3.2.1)
  - [GLM 9.8.5](https://github.com/g-truc/glm/releases/tag/0.9.8.5)
  - [GLEW 2.1.0](https://github.com/nigels-com/glew/releases/tag/glew-2.1.0)
  
### GLFW Installation
- Download source code, use CMake to build VS solution (one per platform). Compile Debug and Release in both Win32 and x64. Libs can be found in `...\glfw-3.2.1\src\$(Configuration)\`
- point to the libraries
- point to the include directory

### GLM Installation
- This is a header only library; no compilation is required though it is possible to compile a .lib and .dll it is not required for this repository
- point to the include directory

### GLEW Installation
- Download the source code, in the `build\` there's a `vc15\glew.sln` which allows for compilation. Debug and Release in both Win32 and x64. The .dll will need to be added to the [correct system](https://stackoverflow.com/a/950011/8480874) directories for applications to run.
- point to the include directory
