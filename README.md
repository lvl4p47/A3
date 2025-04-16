# What is this program?
This is a cellular automaton (CA) on a hexagonal grid.

# Controls
## Mouse
What right, middle and left click do depends on the position of the mouse:
### Center Square
Right and left click change cells according to the values of the cursor. A value of 0 can override any cell, while any other value can override only empty cells.
Middle (mouse wheel) click allows to pan around. Scrolling mouse wheel zooms in/out.
### Toolbar
#### Buttons
![Alt text](/readme_pngs/toolbar.png "")

You can choose what tile you place from a list using scroll wheel and left/right click.
The first button swaps right and left values of the cursor.
The second button on the next row when toggles allows to pan around using left-mouse button.
#### Slider
This slider changes the size of the cursor (brush-size). Setting this size to 0 means that the cursor will change only one cell.  
### Control panel
![Alt text](/readme_pngs/control_panel.png "")

The first button toggles pause state.
The second button does one step of the cellular automaton (only useful while paused).
The third button when toggled allows to see the density of each cell.
The fourth button shifts around the tiles minimap draws.
#### Slider
This slider changes the time between each step of the of the cellular automaton (in ms). Setting this time to 0 means that the cellular automaton will update as fast as it can.
### Minimap
![Alt text](/readme_pngs/minimap.png "")

The minimap shows the amount of non-zero cells at the scale of 1/4.
### Window buttons (in the top-right corner)
![Alt text](/readme_pngs/window_buttons.png "")

The first button minimizes the window.
The second button toggles fullscreen.
The third Button exits the program.
### Information box
![Alt text](/readme_pngs/infobox.png "")

The information box shows different information depending on where you click the mouse.
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
    2. "--" means this neighbor is ignored.
    3. "-x" means the x-cell must NOT be in the neighborhood of the old cell for it to change. For example: setting THREE neighbors to "-2" means that the change will occur only when there is less than THREE 2-cells.
5. Change the flag for this rule:
    1. "--" means this rule is ignored.
    2. "+0" means this rule will work even if the amount of "+x" - neighbours is greater, and even if the amount of "-x" - neighbours is lower.
    3. "+1" means this rule will work only if the amounts of "+x" and "-x" - neighbours are both equal to the amounts required by the conditions.
# Cells
There are 9 types of cells:

### 0 -- Empty cell.
### 1 -- Cloth. 
* Falls.
* Can be suspended by Cloth (1) cells on both sides.
* Can be set on Fire (2).
### 2 -- Fire.
* Ignites Cloth (1), boils Water (3), melts Ice (7).
### 3 -- Water.
* Falls.
* Turns into Ice (7) when in contact with it.
### 4 -- Sand. 
* Falls down, but if this direction is blocked can go to the adjacent cell.
* Allows caves to form.
### 5 -- Dirt. 
* Falls.
* Forms piles.
* Allows caves to form.
* Does not collapse on vertical 1 - tile tunnels at the surface.
### 6 -- Steam.
* Rises. 
* Steam cells repel each other.
* Turns into Water (3) when condensed.
* Turns into Water (3) when in contact with Ice (7).
### 7 -- Ice.
* Falls.
* Forms steep piles in a lattice pattern, can even form an overhang.
* Allows caves to form.
* Turns Water (3) to Ice (7).
* Turns Steam (6) to Water (3). 
* Turns into Water (3) when in contact with Fire (2).
### 8 -- Rock.
* Falls.
* Forms steep piles, can even form an overhang.
* Allows caves to form.
* Does not collapse on vertical and horizontal 1 - tile tunnels at the surface.

