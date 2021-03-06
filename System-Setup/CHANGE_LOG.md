# Change Log 
To track modifications to the system setup application

### Versioning schema
> 0.x.x - Major: Significant rework of functionality

> x.0.x - Minor: New feature or rework of an existing module (without significantly changing UX)

> x.x.0 - Fix:   Small tweaks which do not impact stability

## Setup 1.2.1
- Correcting bad output of filename
- Improved documentation

## Setup 1.2.0
- Script concept is now encapsulated in an object, allows for adding variables, creating file and executing
- Sets Enviroment variables - include directory { GLM, GLEW, GLFW, **FT** } library directory { GLEW, GLFW, **FT** }
- Update `Libraries.zip` to include new FreeType library
- Folder selection remembers previous choice

## Setup 1.1.2
- There was a issue of syntax error, the user was using a path with spaces, this was fixed by adding quatations to the path saved in the script

## Setup 1.1.1
- There was an issue whereby a users system required trailing backslash for windows security reasons, this has been added as the defacto standard for the setup app

## Setup 1.1.0
- Now works even if `setup.bat` exists
- Sets Enviroment variables - include directory { GLM, GLEW, GLFW } library directory { GLEW, **GLFW** }
- corrected typo for GLFW lib env var

## Setup 1.0.0
- Initial Release of `System Setup.exe`
- Supported Platforms: Windows x64
- Sets Enviroment variables - include directory { **GLM**, **GLEW**, **GLFW** } library directory { **GLFW** }
- Console App with WinAPI dialoges
- Creates and Executes Batch File
- Offers Pre-compiled libraries for quick install
