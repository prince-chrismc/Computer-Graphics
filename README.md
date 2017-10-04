# Computer-Graphics
This a C++ OpenGL repository to contain my work for Computer Graphics (COMP371) from Concordia Universirty during Fall 2017.

### Table of Contents
- [Modules](#Modules)
- [System Setup](#System-Setup)
  - [Libraries Installation](#Libraries-Installation)
  - [Setting Enviroment Variables](#Setting-Enviroment-Variables)
  
## Modules
- [System Setup App](https://github.com/prince-chrismc/Computer-Graphics/blob/master/System-Setup)
- [Pseudo Pac-Man](https://github.com/prince-chrismc/Computer-Graphics/blob/master/PacmanA1)
- [Catmull Height Map]()

## System Setup
> Due to how the first assignment was distributed, the Pam-Man project does not use the enviroment variables. It is hardcoded for my workstation's configuration.

- Visual Studio
- OpenGL Libraries
  - [GLFW 3.2.1](https://github.com/glfw/glfw/releases/tag/3.2.1)
  - [GLM 9.8.5](https://github.com/g-truc/glm/releases/tag/0.9.8.5)
  - [GLEW 2.1.0](https://github.com/nigels-com/glew/releases/tag/glew-2.1.0)
  
You are no longer required to download and compile the libraries above; for the System Setup there is a zip which contains all the binaries for x64 (Debug/Release) as well as the include file structures. The Setup application and the libraries are avaliable [here](https://github.com/prince-chrismc/Computer-Graphics/releases/tag/Setup-v1.1.0) for download. For details about [installing the libraries](#Libraries-Installation) or for setting up the [enviroment variables](#Setting-Enviroment-Variables).
  
### Libraries Installation
1. Use the link above to download the `Libraries`
2. Extract the files to the location you desire. Recommended `C:\Users\<your_username>\OpenGL\`
3. Copy the runtime .dll's to the system folder. `C:\Windows\System32\`

### Setting Enviroment Variables
1. Use the link above to download the `System Setup.exe`
2. Run the `System Setup.exe` and follow the directions (ie. select the directories)

Note this app is not fancy; if you don't input a correct directory is will ask you again forever, close the console to exit. If for whatever the apps fails make sure to delete the `setup.bat` file if it was create (same directory which was the app is running from).
