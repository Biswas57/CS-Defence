// CS DEFENCE
//
// This program was written by [Biswas Simkhada]
// on [19/6/23]
//
// In this game, as well as CS Defence, enemies spawn from some start position
// and try to move to some end position via a path on the map. The player's goal
// is to place down towers that attack these enemies as they move. As towers
// destroy enemies, the player earns money that they can use to either upgrade
// or place down new towers.

#include <stdio.h>

#define MAP_ROWS 6
#define MAP_COLUMNS 12

#define MAX_TELEPORTERS 1000
#define DEFAULT_VALUE -1
#define BASIC_TOWER_UPGRADE 300
#define POWER_TOWER_UPGRADE 500

/////////////////////////// USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

enum land_type {
    WATER,
    PATH_START,
    PATH_END,
    PATH_UP,
    GRASS,
    PATH_RIGHT,
    PATH_DOWN,
    PATH_LEFT,
    TELEPORTER
};

enum entity {
    EMPTY,
    ENEMY,
    BASIC_TOWER,
    POWER_TOWER,
    FORTIFIED_TOWER,
};

struct tile {
    enum land_type land;
    enum entity entity;

    int n_enemies;
};

struct teleport {
    int init_row;
    int init_col;
    int dest_row;
    int dest_col;
    enum land_type exit_direction;
    int encapsulated;
};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  FUNCTION PROTOTYPES  ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup_initial(struct tile map[MAP_ROWS][MAP_COLUMNS], int *lives,
                   int *money, int *start_row, int *start_col, int *end_row,
                   int *end_col);
void setup_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_row,
                   int start_col);
void setup_lake(struct tile map[MAP_ROWS][MAP_COLUMNS]);
int valid_point(int row, int column);
void read_path(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_row,
               int start_col, int end_row, int end_col);
void read_commands(struct tile map[MAP_ROWS][MAP_COLUMNS],
                   struct teleport teleporter[MAX_TELEPORTERS],
                   int *num_teleporters, int *lives, int *money, int start_row,
                   int start_col);
void spawn_enemy(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col);
void spawn_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money);
void move_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS],
                  struct teleport teleporter[MAX_TELEPORTERS],
                  int num_teleporters, int *lives, int start_row,
                  int start_col);
int move_enemies_inner(struct tile map[MAP_ROWS][MAP_COLUMNS],
                       struct teleport teleporter[MAX_TELEPORTERS],
                       int num_teleporters, int *lives, int start_row,
                       int start_col);
void upgrade_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money);
void attack_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money);
int attack_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col,
                 int range, int power);
int attack_towers(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col);
int attack_point(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col,
                 int power);
void rain(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void rain_inner(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col,
                int row_origin, int col_origin, int row_spacing,
                int col_spacing);
void flood(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void flood_inner(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void flood_point(struct tile map[MAP_ROWS][MAP_COLUMNS],
                 int flood_map[MAP_ROWS][MAP_COLUMNS], int row, int col);
void teleport(struct tile map[MAP_ROWS][MAP_COLUMNS],
              struct teleport teleporter[MAX_TELEPORTERS], int *num_teleporters,
              int start_row, int start_col);
void teleported_point(struct tile map[MAP_ROWS][MAP_COLUMNS],
                      struct teleport teleporter[MAX_TELEPORTERS],
                      int num_teleporters, int init_row, int init_col,
                      int *dest_row, int *dest_col,
                      enum land_type *exit_direction);
void teleporter_encapsulated(struct teleport teleporter[MAX_TELEPORTERS],
                             int num_teleporters, int init_row, int init_col);
void teleport_init_dest_pos(struct tile map[MAP_ROWS][MAP_COLUMNS],
                            struct teleport teleporter[MAX_TELEPORTERS],
                            int *num_teleporters, int start_row, int start_col,
                            int *row1, int *col1, int *row2, int *col2);
void teleport_clear_path(struct tile map[MAP_ROWS][MAP_COLUMNS],
                         struct teleport teleporter[MAX_TELEPORTERS],
                         int num_teleporters, int init_row, int init_col,
                         int dest_row, int dest_col);

void print_usage(void);
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money);
void print_tile(struct tile tile, int entity_print);

int main(void) {
    int lives;
    int money;
    int start_row;
    int start_col;
    int end_row;
    int end_col;
    struct tile map[MAP_ROWS][MAP_COLUMNS];
    struct teleport teleporter[MAX_TELEPORTERS];
    int num_teleporters = 0;
    initialise_map(map);
    setup_initial(map, &lives, &money, &start_row, &start_col, &end_row,
                  &end_col);
    print_map(map, lives, money);
    setup_enemies(map, start_row, start_col);
    print_map(map, lives, money);
    setup_lake(map);
    print_map(map, lives, money);
    read_path(map, start_row, start_col, end_row, end_col);
    print_map(map, lives, money);
    read_commands(map, teleporter, &num_teleporters, &lives, &money, start_row,
                  start_col);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Put your functions here

// This function is used to setup the initial values of the game. It reads in
// the number of lives, money, start and end points.
void setup_initial(struct tile map[MAP_ROWS][MAP_COLUMNS], int *lives,
                   int *money, int *start_row, int *start_col, int *end_row,
                   int *end_col) {
    printf("Starting Lives: ");
    scanf("%d", lives);
    printf("Starting Money($): ");
    scanf("%d", money);
    do {
        printf("Start Point (row col): ");
        scanf("%d %d", start_row, start_col);
        if (!valid_point(*start_row, *start_col)) {
            printf("Error: Start point is out of bounds. Try again.\n");
        }
    } while (!valid_point(*start_row, *start_col));
    
    do {
        printf("End Point (row col): ");
        scanf("%d %d", end_row, end_col);
        if (!valid_point(*end_row, *end_col)) {
            printf("Error: End point is out of bounds. Try again.\n");
        }
    } while (!valid_point(*end_row, *end_col));

    map[*start_row][*start_col].land = PATH_START;
    map[*end_row][*end_col].land = PATH_END;
}

// This function is used to setup the initial number of enemies at the starting
// point.
void setup_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_row,
                   int start_col) {
    int n_enemies;
    printf("Initial Enemies: ");
    scanf("%d", &n_enemies);
    if (n_enemies > 0) {
        map[start_row][start_col].entity = ENEMY;
        map[start_row][start_col].n_enemies = n_enemies;
    }
}

// This function is used to setup the lake in the map. It reads in the lake's
// position and size, and then sets the land type of the tiles in the lake to
// WATER, if the lake is within the map.
void setup_lake(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    int lake_row;
    int lake_col;
    int lake_height;
    int lake_width;
    printf("Lake Position and Size (row col height width): ");
    scanf("%d %d %d %d", &lake_row, &lake_col, &lake_height, &lake_width);
    if (!valid_point(lake_row, lake_col) ||
        !valid_point(lake_row + lake_height - 1, lake_col + lake_width - 1)) {
        printf("Error: Lake out of bounds, ignoring...\n");
        return;
    }
    for (int row = lake_row; row < lake_row + lake_height; row++) {
        for (int col = lake_col; col < lake_col + lake_width; col++) {
            map[row][col].land = WATER;
        }
    }
}

// This function is used to read in commands from the player and execute them.
// When the player runs out life, the game ends.
void read_commands(struct tile map[MAP_ROWS][MAP_COLUMNS],
                   struct teleport teleporter[MAX_TELEPORTERS],
                   int *num_teleporters, int *lives, int *money, int start_row,
                   int start_col) {
    char command;
    printf("Enter Command (Type ? for help): ");
    int break_flag = 0;
    while (scanf(" %c", &command) != EOF && break_flag == 0) {
        if (command == '?') {
            print_usage();
        } else if (command == 'e') {
            spawn_enemy(map, start_row, start_col);
            print_map(map, *lives, *money);
        } else if (command == 't') {
            spawn_tower(map, money);
            print_map(map, *lives, *money);
        } else if (command == 'm') {
            move_enemies(map, teleporter, *num_teleporters, lives, start_row,
                         start_col);
            print_map(map, *lives, *money);
        } else if (command == 'u') {
            upgrade_tower(map, money);
            print_map(map, *lives, *money);
        } else if (command == 'a') {
            attack_enemies(map, money);
            print_map(map, *lives, *money);
        } else if (command == 'r') {
            rain(map);
            print_map(map, *lives, *money);
        } else if (command == 'f') {
            flood(map);
            print_map(map, *lives, *money);
        } else if (command == 'c') {
            teleport(map, teleporter, num_teleporters, start_row, start_col);
            print_map(map, *lives, *money);
        }

        if (*lives <= 0) {
            printf("Oh no, you ran out of lives!");
            break_flag = 1;
        }
        if (break_flag == 0) {
            printf("Enter Command: ");
        }
    }
    printf("\nGame Over!\n");
}

// This function is used to read in the path that enemies can take from the
// starting point to the end point. It reads in a sequence of directions, and
// sets the land type of the tiles in the path to PATH_UP, PATH_DOWN, PATH_LEFT,
// or PATH_RIGHT, depending on the direction.
void read_path(struct tile map[MAP_ROWS][MAP_COLUMNS], int start_row,
               int start_col, int end_row, int end_col) {
    char direction;
    printf("Enter Path: ");
    while ((start_row != end_row) || (start_col != end_col)) {
        scanf(" %c", &direction);
        if (direction == 'u') {
            map[start_row][start_col].land = PATH_UP;
            start_row--;
        } else if (direction == 'd') {
            map[start_row][start_col].land = PATH_DOWN;
            start_row++;
        } else if (direction == 'r') {
            map[start_row][start_col].land = PATH_RIGHT;
            start_col++;
        } else if (direction == 'l') {
            map[start_row][start_col].land = PATH_LEFT;
            start_col--;
        }
    }
}

// This function is used to spawn an enemy at the starting point. It reads in
// the number of enemies to spawn, and then spawns the specified number of
// enemies at the starting point.
void spawn_enemy(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col) {
    int n_enemies;
    scanf("%d", &n_enemies);
    if (n_enemies > 0) {
        if (map[row][col].entity == EMPTY) {
            map[row][col].entity = ENEMY;
            map[row][col].n_enemies = n_enemies;
        } else if (map[row][col].entity == ENEMY) {
            map[row][col].n_enemies += n_enemies;
        }
    }
}

// This function is used to spawn a tower at a specified location. It reads in
// the position of the tower, and then spawns a tower at that position if the
// position is valid.
void spawn_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money) {
    int row;
    int col;
    scanf("%d %d", &row, &col);
    if (!valid_point(row, col) || map[row][col].land != GRASS ||
        map[row][col].entity != EMPTY || *money < 200) {
        printf(
            "Error: Tower creation unsuccessful. Make sure you have at least "
            "$200 and that the tower is placed on a grass block with no "
            "entity.\n");
        return;
    }
    map[row][col].entity = BASIC_TOWER;
    *money -= 200;
    printf("Tower successfully created!\n");
}

// This function is used to move all the enemies on the map. It reads in the
// number of moves to make, and then moves all the enemies on the map the
// specified number of times along the path.
void move_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS],
                  struct teleport teleporter[MAX_TELEPORTERS],
                  int num_teleporters, int *lives, int start_row,
                  int start_col) {
    int num;
    scanf(" %d", &num);
    int enemy_end_count = 0;
    for (int i = 0; i < num; i++) {
        enemy_end_count += move_enemies_inner(map, teleporter, num_teleporters,
                                              lives, start_row, start_col);
    }
    printf("%d enemies reached the end!\n", enemy_end_count);
}

int move_enemies_inner(struct tile map[MAP_ROWS][MAP_COLUMNS],
                       struct teleport teleporter[MAX_TELEPORTERS],
                       int num_teleporters, int *lives, int start_row,
                       int start_col) {
    int row = start_row;
    int col = start_col;
    int prev_enemies = 0;
    int current_enemies = 0;
    while (map[row][col].land != PATH_END) {
        current_enemies = map[row][col].n_enemies;

        if (prev_enemies != 0) {
            map[row][col].entity = ENEMY;
            map[row][col].n_enemies = prev_enemies;
        } else {
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        }

        if (map[row][col].land == PATH_UP) {
            row--;
        } else if (map[row][col].land == PATH_DOWN) {
            row++;
        } else if (map[row][col].land == PATH_RIGHT) {
            col++;
        } else if (map[row][col].land == PATH_LEFT) {
            col--;
        } else if (map[row][col].land == TELEPORTER) {
            int dest_row = DEFAULT_VALUE;
            int dest_col = DEFAULT_VALUE;
            enum land_type exit_direction;
            teleported_point(map, teleporter, num_teleporters, row, col,
                             &dest_row, &dest_col, &exit_direction);
            if (dest_row != DEFAULT_VALUE && dest_col != DEFAULT_VALUE) {
                row = dest_row;
                col = dest_col;
            }

            if (dest_row == DEFAULT_VALUE && exit_direction == PATH_UP) {
                row--;
            } else if (dest_row == DEFAULT_VALUE &&
                       exit_direction == PATH_DOWN) {
                row++;
            } else if (dest_row == DEFAULT_VALUE &&
                       exit_direction == PATH_RIGHT) {
                col++;
            } else if (dest_row == DEFAULT_VALUE &&
                       exit_direction == PATH_LEFT) {
                col--;
            }
        }
        prev_enemies = current_enemies;
    }

    if (current_enemies != 0) {
        *lives -= current_enemies;
        map[row][col].entity = EMPTY;
        map[row][col].n_enemies = 0;
    }
    return current_enemies;
}

// This function is used to upgrade a tower at a specified location. It reads in
// the position of the tower, and then upgrades the tower at that position if
// the position is valid and the player has enough money. A basic tower costs
// $300 to upgrade to a power tower, and a power tower costs $500 to upgrade to
// a fortified tower. A fortified tower cannot be upgraded further.
void upgrade_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money) {
    int row;
    int col;
    scanf("%d %d", &row, &col);
    if (!valid_point(row, col)) {
        printf("Error: Upgrade target is out-of-bounds.\n");
        return;
    }
    if (map[row][col].entity != BASIC_TOWER &&
        map[row][col].entity != POWER_TOWER &&
        map[row][col].entity != FORTIFIED_TOWER) {
        printf("Error: Upgrade target contains no tower entity.\n");
        return;
    }
    if (map[row][col].entity == FORTIFIED_TOWER) {
        printf("Error: Tower cannot be upgraded further.\n");
        return;
    }
    if ((map[row][col].entity == BASIC_TOWER && *money < 300) ||
        (map[row][col].entity == POWER_TOWER && *money < 500)) {
        printf("Error: Insufficient Funds.\n");
        return;
    }
    if (map[row][col].entity == BASIC_TOWER) {
        map[row][col].entity = POWER_TOWER;
        *money -= BASIC_TOWER_UPGRADE;
    } else if (map[row][col].entity == POWER_TOWER) {
        map[row][col].entity = FORTIFIED_TOWER;
        *money -= POWER_TOWER_UPGRADE;
    }
    printf("Upgrade Successful!\n");
}

// This function is used to attack enemies on the map. It reads in the number of
// times to attack, and then attacks the enemies on the map that number of
// times. Each time an enemy is attacked, it is destroyed and the player earns
// $5.
void attack_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], int *money) {
    int num;
    scanf(" %d", &num);
    int enemies_destroyed = 0;
    for (int i = 0; i < num; i++) {
        for (int row = 0; row < MAP_ROWS; row++) {
            for (int col = 0; col < MAP_COLUMNS; col++) {
                enemies_destroyed += attack_towers(map, row, col);
            }
        }
    }
    *money += enemies_destroyed * 5;
    printf("%d enemies destroyed!\n", enemies_destroyed);
}

// This is a helper function for attack_enemies. It initialates a tower attack
// depending on the type of tower and its range and power.
int attack_towers(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col) {
    int enemies_destroyed = 0;
    if (map[row][col].entity == BASIC_TOWER) {
        enemies_destroyed += attack_tower(map, row, col, 1, 1);
    } else if (map[row][col].entity == POWER_TOWER) {
        enemies_destroyed += attack_tower(map, row, col, 1, 2);
    } else if (map[row][col].entity == FORTIFIED_TOWER) {
        enemies_destroyed += attack_tower(map, row, col, 2, 3);
    }
    return enemies_destroyed;
}

// This is a helper function for attack_enemies. It attacks all the enemies in
// range of the given tower.
int attack_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col,
                 int range, int power) {
    int enemies_destroyed = 0;
    for (int i = 1; i < range + 1; i++) {
        for (int j = 1; j < range + 1; j++) {
            enemies_destroyed += attack_point(map, row - i, col - j, power);
            enemies_destroyed += attack_point(map, row - i, col + j, power);
            enemies_destroyed += attack_point(map, row + i, col - j, power);
            enemies_destroyed += attack_point(map, row + i, col + j, power);
            enemies_destroyed += attack_point(map, row - i, col, power);
            enemies_destroyed += attack_point(map, row + i, col, power);
            enemies_destroyed += attack_point(map, row, col - j, power);
            enemies_destroyed += attack_point(map, row, col + j, power);
        }
    }
    return enemies_destroyed;
}

// This is a helper function for attack_tower. It attacks the enemies at the
// certain point by a given tower.
int attack_point(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col,
                 int power) {
    int enemies_destroyed = 0;
    if (valid_point(row, col) && map[row][col].n_enemies != 0) {
        if (map[row][col].n_enemies <= power) {
            enemies_destroyed += map[row][col].n_enemies;
            map[row][col].n_enemies = 0;
            map[row][col].entity = EMPTY;
        } else {
            enemies_destroyed += power;
            map[row][col].n_enemies -= power;
        }
    }
    return enemies_destroyed;
}

// This function is used to control the rainfall in the game. It reads in the
// parameters of the rainfall, and then calls the rain_inner function to rain on
// the map. The rain_inner function is called for each tile on the map. Rainfall
// doesn't always fall on each tile but rather in a way defined by row and
// column spacing and offset.
void rain(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    int row_spacing;
    int col_spacing;
    int row_offset;
    int col_offset;
    scanf(" %d %d %d %d", &row_spacing, &col_spacing, &row_offset, &col_offset);

    int row_origin = (row_offset % row_spacing + row_spacing) % row_spacing;
    int col_origin = (col_offset % col_spacing + col_spacing) % col_spacing;

    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLUMNS; col++) {
            rain_inner(map, row, col, row_origin, col_origin, row_spacing,
                       col_spacing);
        }
    }
}

// This is a helper function for rain. It rains on the map at a certain point if
// the point is defined by the given parameters. Rainfall will also destroy any
// towers that it lands on, except for fortified towers.
void rain_inner(struct tile map[MAP_ROWS][MAP_COLUMNS], int row, int col,
                int row_origin, int col_origin, int row_spacing,
                int col_spacing) {
    if ((col - col_origin) % col_spacing == 0 && row >= row_origin &&
        col >= col_origin && (row - row_origin) % row_spacing == 0 &&
        (map[row][col].land == GRASS || map[row][col].land == WATER)) {
        map[row][col].land = WATER;
        if (map[row][col].entity == BASIC_TOWER ||
            map[row][col].entity == POWER_TOWER) {
            map[row][col].entity = EMPTY;
        }
    }
}

// This function is used to control the flooding in the game. It reads in the
// number of times flooding will occur, and then calls the flood_inner function.
void flood(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    int num;
    scanf(" %d", &num);
    for (int i = 0; i < num; i++) {
        flood_inner(map);
    }
}

// This is a helper function for flood. If a tile is water, it will flood its
// surronding tiles.
void flood_inner(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    int flood_map[MAP_ROWS][MAP_COLUMNS] = {0};
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLUMNS; col++) {
            if (map[row][col].land == WATER) {
                flood_point(map, flood_map, row - 1, col);
                flood_point(map, flood_map, row + 1, col);
                flood_point(map, flood_map, row, col - 1);
                flood_point(map, flood_map, row, col + 1);
            }
        }
    }
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLUMNS; col++) {
            if (flood_map[row][col] == 1) {
                map[row][col].land = WATER;
            }
        }
    }
}

// This is a helper function for flood_inner. It floods a certain point if it is
// valid and is a grass block. It will also destroy any towers that it floods,
// except for fortified towers.
void flood_point(struct tile map[MAP_ROWS][MAP_COLUMNS],
                 int flood_map[MAP_ROWS][MAP_COLUMNS], int row, int col) {
    if (valid_point(row, col) && map[row][col].land == GRASS) {
        flood_map[row][col] = 1;
        if (map[row][col].entity == BASIC_TOWER ||
            map[row][col].entity == POWER_TOWER) {
            map[row][col].entity = EMPTY;
        }
    }
}

// This function allows the player to create teleporters on the map and move
// from one tile to another. It reads in the position of the teleporting pair
// and adds it to the map.
void teleport(struct tile map[MAP_ROWS][MAP_COLUMNS],
              struct teleport teleporter[MAX_TELEPORTERS], int *num_teleporters,
              int start_row, int start_col) {
    int row1;
    int col1;
    int row2;
    int col2;
    scanf(" %d %d %d %d", &row1, &col1, &row2, &col2);

    if (valid_point(row1, col1) == 0 || valid_point(row2, col2) == 0) {
        printf("Error: Teleporters can only be created on path tiles.\n");
        return;
    }

    if (map[row1][col1].land != PATH_UP && map[row1][col1].land != PATH_DOWN &&
        map[row1][col1].land != PATH_LEFT &&
        map[row1][col1].land != PATH_RIGHT) {
        printf("Error: Teleporters can only be created on path tiles.\n");
        return;
    }

    if (map[row2][col2].land != PATH_UP && map[row2][col2].land != PATH_DOWN &&
        map[row2][col2].land != PATH_LEFT &&
        map[row2][col2].land != PATH_RIGHT) {
        printf("Error: Teleporters can only be created on path tiles.\n");
        return;
    }

    int init_row = row1;
    int init_col = col1;
    int dest_row = row2;
    int dest_col = col2;
    teleport_init_dest_pos(map, teleporter, num_teleporters, start_row,
                           start_col, &init_row, &init_col, &dest_row,
                           &dest_col);

    teleporter[*num_teleporters].init_row = init_row;
    teleporter[*num_teleporters].init_col = init_col;
    teleporter[*num_teleporters].dest_row = dest_row;
    teleporter[*num_teleporters].dest_col = dest_col;
    teleporter[*num_teleporters].encapsulated = 0;
    teleporter[*num_teleporters].exit_direction = map[dest_row][dest_col].land;
    *num_teleporters += 1;

    int row = init_row;
    int col = init_col;

    int dest_entity = map[dest_row][dest_col].entity;
    int dest_enemies = map[dest_row][dest_col].n_enemies;
    teleport_clear_path(map, teleporter, *num_teleporters, row, col, dest_row,
                        dest_col);

    map[init_row][init_col].land = TELEPORTER;
    map[dest_row][dest_col].land = TELEPORTER;
    map[dest_row][dest_col].entity = dest_entity;
    map[dest_row][dest_col].n_enemies = dest_enemies;
}

// This is a helper function for teleport. It is used to determine which
// teleporter is the initial position and which is the destination position.
void teleport_init_dest_pos(struct tile map[MAP_ROWS][MAP_COLUMNS],
                            struct teleport teleporter[MAX_TELEPORTERS],
                            int *num_teleporters, int start_row, int start_col,
                            int *row1, int *col1, int *row2, int *col2) {
    int init_row = *row1;
    int init_col = *col1;
    int dest_row = *row2;
    int dest_col = *col2;

    int row = start_row;
    int col = start_col;

    int break_flag = 0;
    while (map[row][col].land != PATH_END && break_flag == 0) {
        if (row == *row1 && col == *col1) {
            init_row = *row1;
            init_col = *col1;
            dest_row = *row2;
            dest_col = *col2;
            break_flag = 1;
        } else if (row == *row2 && col == *col2) {
            init_row = *row2;
            init_col = *col2;
            dest_row = *row1;
            dest_col = *col1;
            break_flag = 1;
        }
        if (map[row][col].land == PATH_UP) {
            row -= 1;
        } else if (map[row][col].land == PATH_DOWN) {
            row += 1;
        } else if (map[row][col].land == PATH_LEFT) {
            col -= 1;
        } else if (map[row][col].land == PATH_RIGHT) {
            col += 1;
        } else if (map[row][col].land == TELEPORTER) {
            enum land_type exit_direction;
            teleported_point(map, teleporter, *num_teleporters, row, col, &row,
                             &col, &exit_direction);
            if (exit_direction == PATH_UP) {
                row -= 1;
            } else if (exit_direction == PATH_DOWN) {
                row += 1;
            } else if (exit_direction == PATH_LEFT) {
                col -= 1;
            } else if (exit_direction == PATH_RIGHT) {
                col += 1;
            }
        }
    }
    *row1 = init_row;
    *col1 = init_col;
    *row2 = dest_row;
    *col2 = dest_col;
}

// This is a helper function for teleport. It is used to clear the path between
// a pair of teleporters.
void teleport_clear_path(struct tile map[MAP_ROWS][MAP_COLUMNS],
                         struct teleport teleporter[MAX_TELEPORTERS],
                         int num_teleporters, int init_row, int init_col,
                         int dest_row, int dest_col) {
    int row = init_row;
    int col = init_col;
    while (row != dest_row || col != dest_col) {
        if (map[row][col].land == PATH_UP) {
            map[row][col].land = GRASS;
            row -= 1;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        } else if (map[row][col].land == PATH_DOWN) {
            map[row][col].land = GRASS;
            row += 1;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        } else if (map[row][col].land == PATH_LEFT) {
            map[row][col].land = GRASS;
            col -= 1;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        } else if (map[row][col].land == PATH_RIGHT) {
            map[row][col].land = GRASS;
            col += 1;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        } else if (map[row][col].land == TELEPORTER) {
            map[row][col].land = GRASS;
            enum land_type exit_direction;
            teleported_point(map, teleporter, num_teleporters, row, col, &row,
                             &col, &exit_direction);
            teleporter_encapsulated(teleporter, num_teleporters, row, col);
            map[row][col].land = GRASS;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
            if (exit_direction == PATH_UP) {
                row -= 1;
            } else if (exit_direction == PATH_DOWN) {
                row += 1;
            } else if (exit_direction == PATH_LEFT) {
                col -= 1;
            } else if (exit_direction == PATH_RIGHT) {
                col += 1;
            }
        }
    }
}

// This is a function to determine the destination of a teleporter as well as
// its exit direction.
void teleported_point(struct tile map[MAP_ROWS][MAP_COLUMNS],
                      struct teleport teleporter[MAX_TELEPORTERS],
                      int num_teleporters, int init_row, int init_col,
                      int *dest_row, int *dest_col,
                      enum land_type *exit_direction) {
    for (int i = 0; i < num_teleporters; i++) {
        if (teleporter[i].init_row == init_row &&
            teleporter[i].init_col == init_col &&
            teleporter[i].encapsulated == 0) {
            *dest_row = teleporter[i].dest_row;
            *dest_col = teleporter[i].dest_col;
            *exit_direction = teleporter[i].exit_direction;
            return;
        }
    }
}

void teleporter_encapsulated(struct teleport teleporter[MAX_TELEPORTERS],
                             int num_teleporters, int init_row, int init_col) {
    for (int i = 0; i < num_teleporters; i++) {
        if (teleporter[i].init_row == init_row &&
            teleporter[i].init_col == init_col &&
            teleporter[i].encapsulated == 0) {
            teleporter[i].encapsulated = 1;
            return;
        }
    }
}

// This is a general helper function to determine whether a point is valid.
int valid_point(int row, int column) {
    if (row < 0 || row >= MAP_ROWS || column < 0 || column >= MAP_COLUMNS) {
        return 0;
    }
    return 1;
}

/**
 * Initialises map tiles to contain GRASS land and EMPTY entity.
 *
 * Parameters:
 *     map - The map to initialise.
 * Returns:
 *     Nothing.
 */
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLUMNS; ++col) {
            map[row][col].land = GRASS;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        }
    }
}

/**
 * Prints all map tiles based on their value, with a header displaying lives
 * and money.
 *
 * Parameters:
 *     map   - The map to print tiles from.
 *     lives - The number of lives to print with the map.
 *     money - The amount of money to print with the map.
 * Returns:
 *     Nothing.
 */
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money) {
    printf("\nLives: %d Money: $%d\n", lives, money);
    for (int row = 0; row < MAP_ROWS * 2; ++row) {
        for (int col = 0; col < MAP_COLUMNS; ++col) {
            print_tile(map[row / 2][col], row % 2);
        }
        printf("\n");
    }
}

/**
 * Prints either the land or entity component of a single tile, based on
 * the `land_print` parameter;
 *
 * Parameters:
 *     tile         - The tile to print the land/entity from
 *     land_print - Whether to print the land part of the tile or the entity
 *         part of the tile. If this value is 0, it prints the land, otherwise
 *         it prints the entity.
 * Returns:
 *     Nothing.
 */
void print_tile(struct tile tile, int land_print) {
    if (land_print) {
        if (tile.land == GRASS) {
            printf(" . ");
        } else if (tile.land == WATER) {
            printf(" ~ ");
        } else if (tile.land == PATH_START) {
            printf(" S ");
        } else if (tile.land == PATH_END) {
            printf(" E ");
        } else if (tile.land == PATH_UP) {
            printf(" ^ ");
        } else if (tile.land == PATH_RIGHT) {
            printf(" > ");
        } else if (tile.land == PATH_DOWN) {
            printf(" v ");
        } else if (tile.land == PATH_LEFT) {
            printf(" < ");
        } else if (tile.land == TELEPORTER) {
            printf("( )");
        } else {
            printf(" ? ");
        }
    } else {
        if (tile.entity == EMPTY) {
            printf("   ");
        } else if (tile.entity == ENEMY) {
            printf("%03d", tile.n_enemies);
        } else if (tile.entity == BASIC_TOWER) {
            printf("[B]");
        } else if (tile.entity == POWER_TOWER) {
            printf("[P]");
        } else if (tile.entity == FORTIFIED_TOWER) {
            printf("[F]");
        } else {
            printf(" ? ");
        }
    }
}

/** 
 * Prints the CS Defence usage instructions, 
 * displaying the different commands and their arguments.
 */
void print_usage(void) {
    printf(
        "=====================[ CS Defence Game ]=====================\n"
        "     ===============[     Command List     ]===============   \n"
        "  e                                                            \n"
        "    Spawn an enemy at the starting position                   \n"
        "  t                                                            \n"
        "    Place a new tower on the map                              \n"
        "  m                                                            \n"
        "    Move all enemies towards the end point                    \n"
        "  u                                                            \n"
        "    Upgrade a tower to increase its effectiveness             \n"
        "  a                                                            \n"
        "    Attack enemies within tower range                         \n"
        "  r                                                            \n"
        "    Cause rain to alter the terrain and affect towers         \n"
        "  f                                                            \n"
        "    Flood the terrain, affecting path and towers              \n"
        "  c                                                            \n"
        "    Create a teleporter between two points on the map         \n"
        "  ?                                                            \n"
        "    Show this help message                                    \n"
        "==============================================================\n"
    );
}