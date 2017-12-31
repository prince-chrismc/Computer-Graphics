# Computer-Graphics
This a C++ OpenGL repository to contain my work for Computer Graphics (COMP371) from Concordia Universirty during Fall 2017.

### Table of Contents
- [Modules](#Modules)
- [System Setup](#System-Setup)
  - [Libraries Installation](#Libraries-Installation)
  - [Setting Enviroment Variables](#Setting-Enviroment-Variables)
  
## Modules

#### [System Setup](https://github.com/prince-chrismc/Computer-Graphics/tree/master/System-Setup)
- [Application](https://github.com/prince-chrismc/Computer-Graphics/tree/master/System-Setup/System-Setup)
- [Project Template](https://github.com/prince-chrismc/Computer-Graphics/tree/master/System-Setup/Template)

#### [Assignments](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Assignments)
- [Pseudo Pac-Man](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Assignments/Pseudo-PacMan)
- [Image Height Map](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Assignments/Image-Height-Map)
- [Ray Tracing](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Assignments/Ray-Tracing)

#### [Libraries](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Libraries)
- [Shader](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Libraries/Shader)
- [Window](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Libraries/Window)
- [Object Loader](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Libraries/ObjectLoader)

## System Setup
- Visual Studio 15.2+ (though any c++14 compliant should work)
- Open-Source Libraries
  - [GLFW 3.2.1](https://github.com/glfw/glfw/releases/tag/3.2.1)
  - [GLM 9.8.5](https://github.com/g-truc/glm/releases/tag/0.9.8.5)
  - [GLEW 2.1.0](https://github.com/nigels-com/glew/releases/tag/glew-2.1.0)
  - [CImg 2.1.0](https://github.com/dtschump/CImg/blob/4182d20c9102fbcc50b04f4780dbc9f5408f1732/CImg.h)
  - [FreeType 2.8.1](https://www.freetype.org/)
  
You are no longer required to download and compile the libraries above; for the _System Setup_ there is a .zip which contains all the binaries for x64 (Debug/Release) as well as the include file structures. The application and the libraries are avaliable [here](https://github.com/prince-chrismc/Computer-Graphics/releases/tag/Setup-v1.2.1) for download. For details about [installing the libraries](#Libraries-Installation) or for setting up the [enviroment variables](#Setting-Enviroment-Variables).
  
### Libraries Installation
1. Use the link above to download the `Libraries`
2. Extract the files to the location you desire. Recommended `C:\Users\<your_username>\OpenGL\`
3. Copy the runtime .dll's to the system folder. `C:\Windows\System32\`

### Setting Enviroment Variables
1. Use the link above to download the `System Setup.exe`
2. Run the `System Setup.exe` and follow the directions displayed. For more [information](https://github.com/prince-chrismc/Computer-Graphics/blob/master/System-Setup/README.md), use the link provided.
