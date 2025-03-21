# What is this program?
This is a cellular automaton (CA) on a hexagonal grid.

# Controls
## Mouse
What Right, Middle and Left click do depends on the position of the mouse:
### Center Square
Right and Left click changes cells according to the values of the cursor in the top-left corner of the screen. Value equal to 0 can override any cell, while any other value can override only empty cells.
Middle (Mouse wheel) click allows to pan around. Scrolling Mouse wheel rotates the grid.
### Buttons
#### Buttons on the left side of the screen
![Alt text](/readme_pngs/Button_circle.png "")
This Button adds 1 to the value of the cursor (right-click adds to the right value of the cursor, left-click adds to the left value of the cursor).

![Alt text](/readme_pngs/Button_eraser.png "")
This Button sets corresponding cursor value to 0 (right-click sets the right value of the cursor, left-click sets the left value of the cursor).
#### Buttons on the right side of the screen
![Alt text](/readme_pngs/Button_pause.png "")
This Button toggles pause state.

![Alt text](/readme_pngs/Button_step.png "")
This Button does one step of the cellular automaton (only useful while paused).

![Alt text](/readme_pngs/Button_exit.png "")
This Button (in the top-right corner) exits the program.
### Sliders
#### Sliders on the left side of the screen
![Alt text](/readme_pngs/Slider_size.png "")
This slider changes the size of the cursor (brush-size). Setting this size to 0 means that the cursor will change only one cell.
#### Sliders on the right side of the screen
![Alt text](/readme_pngs/Slider_time.png "")
This slider changes the time between each step of the of the cellular automaton (in ms). Setting this time to 0 means that the cellular automaton will update as fast as it can.
# Cells
There are 9 types of cells:

### 0 -- empty cell.
### 1 -- Rope. 
* Falls.
* Can be suspended by Rope (1) cells on both sides.
* Can be set on Fire (2).
### 2 -- Fire.
* Ignites Rope (1), boils Water (3), melts Ice (7).
### 3 -- Water.
* Falls.
* Turns Dirt (5) and Rock (8) to Sand (4).
* Turns into Ice (7) when in contact with it.
### 4 -- Sand. 
* Falls down, but if this direction is blocked can go to the adjacent empty cell.
* Turns into Dirt (5) when in contact with Dirt (5).
### 5 -- Dirt. 
* Falls.
* Can be supported by other cells on both sides.
* Does not collapse on 1 - tile tunnels.
* Turns Sand (4) to Dirt (5).
* Turns into Sand (4) when in contact with Water (3).
### 6 -- Steam.
* Rises. 
* Steam cells repel each other.
* Turns into Water (3) when in contact with other Steam (6).
* Turns into Ice (7) when in contact with Ice (7).
### 7 -- Ice.
* Falls.
* Can be supported by other cells.
* Forms a lattice.
* Turns Water (3) and Steam (6) to Ice (7). 
* Turns into Water (3) when in contact with Fire (2).
### 8 -- Rock.
* Turns into Sand (4) when in contact with Water (3).

