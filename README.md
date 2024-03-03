# CS Defence | Bloons Tower Defence Emulator

## Description
CS Defence is a terminal-based strategy game where players place towers to defend against incoming enemies. As enemies are defeated, players earn money to upgrade and place new towers. It's supposed to be base off an extremely popular mobile and online game called Bloons Tower Defence

## Compilation and Running
1. Clone this repository to your local machine using $ **git clone _repository-url_**
2. Navigate to the cloned directory using $ **cd CS-Defence**
3. Compile the game using $ **gcc -o cs_defence cs_defence.c**
4. Run the compiled code using $ **./cs_defence**

### Game Instructions
#### Starting the Game
When you start the game, you will be prompted to set up the initial game settings:

Starting Lives: The number of lives you start with. You lose a life for each enemy that reaches the end of the path.
Starting Money: The amount of money you have to spend on placing and upgrading towers.
Start Point: The coordinates (row and column) where enemies will start.
End Point: The coordinates (row and column) where enemies will aim to reach.


#### Game Commands
After setting up the game, you will enter the main game loop where you can input commands to interact with the game:

e: Spawn an enemy at the starting position.
t: Place a new tower on the map. You will be prompted to enter the coordinates where you want to place the tower and ensure you have enough money.
m: Move all enemies towards the end point. Enemies move along the path you've defined.
u: Upgrade a tower to increase its effectiveness. You will be prompted to enter the coordinates of the tower you want to upgrade.
a: Attack enemies within tower range. Towers automatically attack enemies, but you can use this command to simulate tower attacks.
r: Cause rain to alter the terrain and affect towers. This can change grass tiles to water.
f: Flood the terrain, affecting the path and towers. This can spread water further, affecting the path's navigability for enemies.
c: Create a teleporter between two points on the map. You will be prompted to enter the coordinates for both points of the teleporter.
?: Show help message with these commands.


#### Playing the Game
Designate a Path: Before spawning enemies, use the environment to your advantage by designing a path from the start point to the end point. You can't directly input this path, but keep it in mind as you place towers and obstacles.

Place Towers: Use your starting money to place towers strategically along the path. Consider choke points or areas where you can maximize the towers' effectiveness.

Spawn Enemies: Spawn enemies to start the assault. Monitor how well your towers perform and adjust your strategy accordingly.

Upgrade Towers: As you earn more money from defeating enemies, consider upgrading your towers to enhance their attack power and range.

Utilize the Environment: Use rain and flood commands to modify the terrain, potentially slowing down enemies or forcing them into more advantageous positions for your towers.

Teleporters: Strategically place teleporters to redirect enemies, potentially leading them into prolonged paths or more heavily defended areas.

