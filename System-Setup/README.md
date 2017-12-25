## System Setup
This section of the repository is to facilitate the configuration of the enviroment variables used by the OpenGL app in the [Assignments](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Assignments) section. The reason to go this route was base upon having multiple machines being used during the development process; all of which required the libraries may of which being installed in different locations -- thus hardcoded paths would have possed challenges.

##### Warnings
This app is very _limited_ in terms of functionality. It is looking for the file tree like [show below](#file-tree). **WARNIGN:** The testing/veryfication of you selection is checking only a single file.

##### Recommendations
The [`Libraries.zip`](https://github.com/prince-chrismc/Computer-Graphics/releases/download/Setup-v1.2.0/Libraries.zip) pacakge provided in the releases is tailored for the setup app. The template is a basic outline for any project and is very generic and is built using the enviroment variables. It is **HIGHLY RECOMENDED** To use the .zip provided when configuring the enviroment variables.

### File Tree
```
Libraries
+---freetype-2.8.1
|   +---include                                   // FT_INC_DIR
|   |   \---freetype
|   |       +---config
|   |       \---internal
|   |           \---services
|   \---lib                                       // FT_LIB_DIR
|       +---Debug
|       \---Release
+---glew-2.1.0
|   +---bin                                       // Contents of these sub-folders go to sys32 folder
|   |   +---Debug
|   |   \---Release
|   |       \---Win32
|   +---include                                   // GLEW_INC_DIR
|   |   \---GL
|   \---lib                                       // GLEW_LIB_DIR
|       +---Debug
|       \---Release
+---glfw-3.2.1
|   +---bin                                       // GLFW_LIB_DIR
|   |   +---Debug
|   |   \---Release
|   \---include                                   // GLFW_INC_DIR
|       +---glad
|       \---GLFW
\---glm-9.8.5
    \---glm                                       // GLM_INC_DIR
        +---detail
        +---gtc
        +---gtx
        \---simd
```

### Screenshot
![image](https://user-images.githubusercontent.com/16867443/31201517-f1a53fba-a92c-11e7-93f9-8553e5f03a3a.png)
