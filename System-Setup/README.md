# System Setup
This Section of the repository is to facilitate configure the enviroment variables. The app is very limited in terms of functionality. The app is looking for a file tree like [this](#file-tree-example); however the testing is only a single base file for each module. The [`Libraries.zip`](https://github.com/prince-chrismc/Computer-Graphics/releases/download/Setup-v1.1.0/Libraries.zip) pacakge provided in the releases is tailored for the setup app. The template is a basic outline for any project and is very generic but is built using the enviroment variables set by the setup.

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
