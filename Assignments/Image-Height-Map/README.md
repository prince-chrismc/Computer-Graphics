# Image Height Map
This appilcation reads a bitmap image converting the "pixel value" to a height and generating a 3D height map. To add the prespective of depth, the colors range from dark blue to green. The outter points are used but no additional points are simulated to maintain the maps aspect ration. Behind the scenes, the app is running an object-oriented abstraction at almost all levels, leaving only the spline calculation and image reading to procedual code.

### Table Of Contents
* [Contributors](#contributors)
* [Sequence](#sequence)
* [Controls](#controls)
  + [General](#general)
  + [Render Mode](#render-mode)
  + [Camera - Keyboard](#keyboard)
  + [Camera - Mouse](#mouse)
  
## Contributors
**Student** | **ID**
:---:| ---
Christopher McArthur | `40004257`

## Sequence
On start-up, the app loads the image's pixel values (using `CImg.data()`) storing the points, calculating the color, and connecting the points to form triangles. Following which the user is promted to enter the values, skip size and step size, and the spline height maps are interpolated using the Catmul-Rom method. Users can now interact with the models, moving the camera around, toggling the maps, changes the render mode, and generate new maps.

## Controls

### General
**Action** | **Effect**
:---:| ---
ESC_KEY | Exit the game
BACKSPACE_KEY | Add new catmul spline maps (resets camera)

### Render Mode
**Action** | **Effect**
:---:| ---
P_KEY | Render objects as points only
T_KEY | Render objects normally

### Camera

#### Keyboard
**Action** | **Effect**
:---:| ---
W_KEY | Translate-X
A_KEY | Translate-Z
S_KEY | Translate-Z
D_KEY | Translate-X
UP_KEY | Rotate-Z
LEFT_KEY | Rotate-X
DOWN_KEY | Rotate-X
RIGHT_KEY | Rotate-Z
HOME_KEY | Reset Camera

#### Mouse
**Trigger** | **Action** | **Effect**
:---: | :---: | ---
LEFT_BUTTON | Cursor Y-axis mouvement | Zoom in (move forward), Zoom out (pull backwards)
RIGHT_BUTTON | Cursor Y-axis mouvement | Rotate along board Y-axis
