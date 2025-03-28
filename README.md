# What is this program?
This is a cellular automaton (CA) on a hexagonal grid.

# Controls
## Mouse
What Right, Middle and Left click do depends on the position of the mouse:
### Center Square
Right and Left click changes cells according to the values of the cursor in the top-left corner of the screen. Value equal to 0 can override any cell, while any other value can override only empty cells.
Middle (Mouse wheel) click allows to pan around. Scrolling Mouse wheel rotates the grid.
### Toolbar
#### Buttons
![Alt text](/readme_pngs/toolbar.png "")
The first Button adds 1 to the value of the cursor (right-click adds to the right value of the cursor, left-click adds to the left value of the cursor).
The second Button sets corresponding cursor value to 0 (right-click sets the right value of the cursor, left-click sets the left value of the cursor).
The third button swaps right and left values of the cursor.
The fourth button on the next row when toggles allows to pan around using left-mouse button.
#### Slider
This slider changes the size of the cursor (brush-size). Setting this size to 0 means that the cursor will change only one cell.  
### Control panel
![Alt text](/readme_pngs/control_panel.png "")
The first Button toggles pause state.
The second Button does one step of the cellular automaton (only useful while paused).
The third button when toggled allows to see the density of each cell.
#### Slider
This slider changes the time between each step of the of the cellular automaton (in ms). Setting this time to 0 means that the cellular automaton will update as fast as it can.
### Window buttons (in the top-right corner)
![Alt text](/readme_pngs/window_buttons.png "")
The first button minimizes the window.
The second button toggles fullscreen.
The third Button exits the program.
### Rule editor
![Alt text](/readme_pngs/rules_editor.png "")
The rule editor allows to change the rules of the CA.
#### How to use it
Follow steps in this order:
1. Select the cell you want to change - the old cell (in the center of the hexagon). 
2. Select the cell you want the old cell to change into - the new cell (in the center to the right).
3. Select the index of the conditions for this change (bottom-right). If you want to keep already existing conditions, add new conditions with a + button.
4. Edit the neighboring cell that are required for this change:
    1. "+x" means the x-cell is required to be in the neighborhood of the old cell for it to change into the new cell.
    2. "--"" means this neighbor is ignored.
    3. "-x" means the x-cell must NOT be in the neighborhood of the old cell for it to change. For example: setting THREE neighbors to "-2" means that the change will occur only when there is less than THREE 2-cells.


# Cells
There are 9 types of cells:

### 0 -- Empty cell.
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
* Can be supported by other cells on both sides and forms caves.
* Does not collapse on 1 - tile tunnels.
* Turns Sand (4) to Dirt (5).
* Turns into Sand (4) when in contact with Water (3).
### 6 -- Steam.
* Rises. 
* Steam cells repel each other.
* Turns into Water (3) when in contact with other Steam (6).
* Turns into Water (3) when in contact with Ice (7).
### 7 -- Ice.
* Falls.
* Can be supported by other cells.
* Forms a lattice.
* Turns Water (3) to Ice (7).
* Turns Steam (6) to Water (3). 
* Turns into Water (3) when in contact with Fire (2).
### 8 -- Rock.
* Falls.
* Forms steep piles, can even form a vertical wall if you carefully remove it at the foot.
* Does not collapse on 1 - tile tunnels.
* Turns into Sand (4) when in contact with Water (3).

