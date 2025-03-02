# What is this program?
	This is a cellular automaton (CA) on a hexagonal grid.

# Controls
## Mouse
What Right, Middle and Left click do depends on the position of the mouse:
### Center Square
Right and Left click changes cells according to the values of the cursor in the top-left corner of the screen. Value equal to 0 can override any cell, while any other value can only override empty cells.
Middle (Mouse wheel) click allows to pan around. Scrolling Mouse wheel rotates the grid.
### Buttons
#### Buttons on the left side of the screen
(/readme/Button_circle.png "") -- This Button adds 1 to the value of the cursor (right-click adds to the right value of the cursor, left-click adds to the left value of the cursor).
(/readme/Button_eraser.png "") -- This Button sets corresponding cursor value tp 0 (right-click sets the right value of the cursor, left-click sets the left value of the cursor).
#### Buttons on the right side of the screen
(/readme/Button_pause.png "") -- This Button toggles pause state.
(/readme/Button_step.png "") -- This Button does one step of the cellular automaton (only useful while paused).
(/readme/Button_exit.png "")-- This Button (in the top-right corner) exits the program.
### Sliders
#### Sliders on the left side of the screen
(/readme/Slider_size.png "") -- This slider changes the size of the cursor (brush-size). Setting this size to 0 means that the cursor will change only one cell.
#### Sliders on the right side of the screen
(/readme/Slider_time.png "") -- This slider changes the time between each step of the of the cellular automaton (in ms). Setting this time to 0 means that the cellular automaton will update as fast as it can.
# Cells
There are 7 types of cells:

0 -- empty cell.

1 -- Wall. Can be set on Fire (2). Turns water (3) to Wall (1).

2 -- Fire. Lights Wall (1) and Gas (6).

3 -- Water. Water cells repel each other. Turns into Wall (1) when in contact with it.

4 -- Sand. Falls down, but if this direction is blocked can go to the adjacent empty cell.

5 -- Dirt. Falls down, if there isn't supporting cells nearby.

6 -- Gas. Gas cells repel each other. Can be set on Fire (2).
