## System Setup
This section of the repository is to facilitate the configuration of the enviroment variables used by the OpenGL app in the [Assignments](https://github.com/prince-chrismc/Computer-Graphics/tree/master/Assignments) section. The reason to go this route was base upon having multiple machines being used during the development process; all of which required the libraries may of which being installed in different locations -- thus hardcoded paths would have possed challenges.

##### Warnings
This app is very _limited_ in terms of functionality. It is looking for the file tree like [show below](#file-tree). **WARNING:** The testing/veryfication of you selection is checking only a single file.

It is possible for the script to terminate successfully while having errors reported. The most likely is the "Invalid Syntax" which can be seen [here](#False-Positive-Termination). _Please_ report this issue and lable it a bug and attach the following information
* Screenshot of the console
* Copy of the setup.bat (should be present with the app)
* And the path to which you are trying to save

##### Recommendations
The [`Libraries.zip`](https://github.com/prince-chrismc/Computer-Graphics/releases/download/Setup-v1.2.1/Libraries.zip) pacakge provided in the releases is tailored for the setup app. The template is a basic outline for any project and is very generic and is built using the enviroment variables. It is **HIGHLY RECOMENDED** To use the .zip provided when configuring the enviroment variables.

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

###### Successful Termination
![image](https://user-images.githubusercontent.com/16867443/34343481-fbb2fb94-e99d-11e7-8cc1-2b77a4d7fa0f.png)

###### General
![image](https://user-images.githubusercontent.com/16867443/31201517-f1a53fba-a92c-11e7-93f9-8553e5f03a3a.png)

###### Remembers Previous Selection
![image](https://user-images.githubusercontent.com/16867443/34343423-0ec33cf0-e99c-11e7-824d-65646c833305.png)

###### Failed Selection
![image](https://user-images.githubusercontent.com/16867443/34343437-5a975152-e99c-11e7-83f2-e1157a9b66fd.png)

###### False Positive Termination
![image](https://user-images.githubusercontent.com/16867443/34343499-e90899bc-e99e-11e7-850a-d155261df3d2.png)
