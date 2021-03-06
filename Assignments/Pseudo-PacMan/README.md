# Pseudo Pac-Man
This is an sort of pac-man logic mini game, where the main character moves around the board eating *food* and avoiding aliens. But dont get scared off by the aliens they dont move!
>*To view this [document](https://github.com/prince-chrismc/Computer-Graphics/blob/master/Assignments/Pseudo-PacMan/README.md) properly, click on the link provided.*

### Table Of Contents
* [Contributors](#contributors)
* [Game Options](#game-options)
* [Controls](#controls)
  + [General](#general)
  + [Pac-Man](#pac-man)
  + [Render Mode](#render-mode)
  + [Object Scaling](#object-scaling)
  + [Camera - Keyboard](#keyboard)
  + [Camera - Mouse](#mouse)

## Contributors
**Student** | **ID**
:---:| ---
Christopher McArthur | `40004257`

## Game Options
**Option** | **Effect** | **Limit**
:---:| --- | ---
Board Size | Allows the user to select a grid size before game launch | Range: [9, 35], Condition: Must be an odd number

## Controls

### General
**Action** | **Effect**
:---:| ---
ESC_KEY | Exit the game

### Pac-Man
**Action** | **Effect**
:---:| ---
W_KEY | ++Translate-X
A_KEY | --Translate-Y
S_KEY | ++Translate-Y
D_KEY | --Translate-X

### Render Mode
**Action** | **Effect**
:---:| ---
P_KEY | Render objects as points only
L_KEY | Render objects with lines only
T_KEY | Render objects normally

### Object Scaling
**Action** | **Effect** | **Limit**
:---:| --- | ---
U_KEY | Increase the size of the objects | Max: x10
J_KEY | Decrease the size of the objects | Min: x0

### Camera

#### Keyboard
**Action** | **Effect**
:---: | ---
UP_KEY | ++Translate-Y
LEFT_KEY | --Translate-X
DOWN_KEY | --Translate-Y
RIGHT_KEY | ++Translate-X

#### Mouse
**Trigger** | **Action** | **Effect**
:---: | :---: | ---
LEFT_BUTTON | Cursor X-axis mouvement | Zoom in (towards right), Zoom out (towards left)
MIDDLE_BUTTON | Cursor Y-axis mouvement | Tilt along board along Z-axis (up/down relative to the left side ofthe board)
RIGHT_BUTTON | Cursor X-axis mouvement | Pan along board X-axis (drag && drop style; right-to-right, left-to-left)
