# CS Defence | Bloons Tower Defence Emulator

## Description
CS Defence is a two-player terminal-based strategy game inspired by the popular mobile and online game Bloons Tower Defence. In this game, one player controls the placement and upgrade of towers to defend against incoming enemies, while the second player spawns and directs the enemies attempting to breach the defenses. As enemies are defeated, the defending player earns money to upgrade and place more towers, enhancing their strategic depth and defense capabilities.

## Compilation and Running
1. Clone this repository to your local machine using $ **git clone _repository-url_**
2. Navigate to the cloned directory using $ **cd CS-Defence**
3. Compile the game using $ **gcc -o cs_defence cs_defence.c**
4. Run the compiled code using $ **./cs_defence**

### Game Instructions
#### Starting the Game
When you start the game, you will be prompted to set up the initial game settings:

**Starting Lives:** The number of lives you start with. You lose a life for each enemy that reaches the end of the path.
**Starting Money:** The amount of money you have to spend on placing and upgrading towers.
**Start Point:** The coordinates (row and column) where enemies will start.
**End Point:** The coordinates (row and column) where enemies will aim to reach.

#### Defining a Path
You will be prompted to enter a sequence of directions to create a path. Each direction corresponds to a movement from the current tile to an adjacent tile:

- 'u' (up): Move one tile up.
- 'd' (down): Move one tile down.
- 'l' (left): Move one tile left.
- 'r' (right): Move one tile right.

Enter the directions consecutively without spaces. For example, if the path should start by moving up twice, then right three times, you would input `uurr`.

#### Game Commands
After setting up the game, you will enter the main game loop where you can input commands to interact with the game:

- `e`: **Enemy player** spawns an enemy at the starting position.
- `t (row col)`: Place a new tower on the map. You will be prompted to enter the coordinates where you want to place the tower and ensure you have enough money.
- `m (no_steps)`: Move all enemies towards the end point. Enemies move along the path you've defined.
- `u (row col)`: Upgrade a tower to increase its effectiveness. You will be prompted to enter the coordinates of the tower you want to upgrade.
- `a (no_attacks)`: Attack enemies within tower range. Towers automatically attack enemies, but you can use this command to simulate tower attacks.
- `r (row col row_offset col_offset)`: Cause rain to alter the terrain and affect towers. This can change grass tiles to water.
- `f (no_times)`: Flood the terrain, affecting the path and towers. This can spread water further, affecting the path's navigability for enemies.
- `c (row col dest_row dest_col)`: Create a teleporter between two points on the map. You will be prompted to enter the coordinates for both points of the teleporter.
- `?`: Show help message with these commands.


#### Playing the Game
**Designate a Path:** Before spawning enemies, the enemy player should strategize the best route to challenge the defenses.

**Place Towers:** The defending player uses the starting money to place towers strategically along the path. Consider choke points or areas where you can maximize the towers' effectiveness.

**Spawn Enemies:** The enemy player spawns enemies to start the assault. Both players should monitor how well the towers perform and adjust their strategies accordingly.

**Upgrade Towers:** As the defending player earns more money from defeating enemies, they should consider upgrading their towers to enhance their attack power and range.

**Utilize the Environment:** The defending player can use rain and flood commands to modify the terrain, potentially slowing down enemies or forcing them into more advantageous positions for your towers.

**Teleporters:** The defending player can strategically place teleporters to redirect enemies, potentially leading them into prolonged paths or more heavily defended areas.
