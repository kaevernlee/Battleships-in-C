#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

#define MAX_INPUT 50

#define ROW_MAX 8
#define COL_MAX 8
#define PLAYERS_MAX 2
#define PLAYER_BOARDS 2

#define SHIPS 0
#define SHOTS 1

#define NUM_OF_SHIPS 5

#define CARRIER_CELLS 5
#define BATTLESHIP_CELLS 4
#define DESTROYER_CELLS 3
#define SUBMARINE_CELLS 2
#define PB_CELLS 2


/**
 * NOTE: There are two players and each player has 2 boards.
 * Board 1) Ship placement
 * Board 2) Shots fired
 */
char** player[PLAYERS_MAX][PLAYER_BOARDS];
char player_ship_coordinates[PLAYERS_MAX][NUM_OF_SHIPS][MAX_INPUT];
const char* ships[] = {
        "Carrier",
        "Battleship",
        "Destroyer",
        "Submarine",
        "Patrol Boat"
};
const int ships_cell_value[] = {
        CARRIER_CELLS,
        BATTLESHIP_CELLS,
        DESTROYER_CELLS,
        SUBMARINE_CELLS,
        PB_CELLS
};
int sunk_ships[PLAYERS_MAX][NUM_OF_SHIPS] = {
        false,
        false,
        false,
        false,
        false
};

char** initBoard() {
    // allocate dynamic memory for board via board pointer to arrays
    char** board = malloc(ROW_MAX * sizeof(char *));             // allocate memory the row pointers
    board[0] = malloc(ROW_MAX * COL_MAX * sizeof(char));         // allocate memory all the elements in row i.e. col
    for(int i = 1; i < ROW_MAX; i++)
        board[i] = board[0] + i * COL_MAX;

    // initialise all positions to white space
    int row, col;
    for (row = 0; row <  ROW_MAX; row++) {
        for (col = 0; col < COL_MAX; col++) {
            board[row][col] = ' ';
        }
    }
    return board;
}

void clean(char** board) {
    // free all malloc allocations
    free(*board);
}

char getBoard(char** board, int row, int col) {
    return board[row][col];
}

void setBoard(char** board, char c, int row, int col) {
    // change a single character in the board
    board[row][col] = c;
}

void toString(char** board) {
    // print col numbers first
    printf("\n  1 2 3 4 5 6 7 8\n");

    // print board to string
    int row, col;
    for (row = 0; row < ROW_MAX; row++) {
        for (col = 0; col < COL_MAX; col++) {
            if (col == 0) {
                printf("%c|%c|", 65+row, board[row][col]);
            } else if (col == COL_MAX-1) {
                printf("%c|\n", board[row][col]);
            } else {
                printf("%c|", board[row][col]);
            }
        }
    }
    printf("\n");
}

void set_ship(int p, int ship, char coordinate[]) {
    strcpy(player_ship_coordinates[p][ship], coordinate);
}

// places ship on player's board
int placeShip(char** board, int shipCells, char input[]) {
    // grid logic
    int row = input[0] - 65;            // convert char into index via ascii
    int col = input[1] - '0' - 1;       // convert char of a string into int
    char direction = input[2];

    // if the input format is incorrect
    if ((row < 0) || (row > 7)) {
        printf("Invalid ship configuration\n");
        return false;
    }
    if ((col < 0) || (col > 7)) {
        printf("Invalid ship configuration\n");
        return false;
    }
    if (direction != 'H') {
        if (direction != 'V') {
            return false;
        }
    }


    // if column doesn't fit ship placement
    if ((shipCells+col) > COL_MAX) {
        printf("Invalid ship configuration\n");
        return false;
    }
    if ((shipCells+row) > ROW_MAX) {
        printf("Invalid ship configuration\n");
        return false;
    }

    // updating board
    if (direction == 'H') {
        for (int i = 0; i < shipCells; i++) {
            // if ship already placed down
            if (getBoard(board, row, col+i) == '*') {
                printf("Invalid ship configuration\n");
                return false;
            }
            setBoard(board, '*', row, col+i);
        }
    }
    else if (direction == 'V') {
        for (int i = 0; i < shipCells; i++) {
            if (getBoard(board, row+i, col) == '*') {
                printf("Invalid ship configuration\n");
                return false;
            }
            setBoard(board, '*', row+i, col);
        }
    }
    return true;
}


/***************************************************************************************************
*    Title: StackOverFlow - 'Removing Spaces from a String in C'                                   *
*    Author: ericcurtin                                                                            *
*    Date: 07/2019                                                                                 *
*    Code version: v2.0                                                                            *
*    Availability: https://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c  *
****************************************************************************************************/
void remove_white(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ( (*s++ = *d++) );
}

void print_coordinates() {
    int p_test = 0;
    while (p_test < 2) {
        for (int i = 0; i < NUM_OF_SHIPS; i++) {
            printf("\nPlayer %d testing putting coordinate in array: %s\n", p_test, player_ship_coordinates[p_test][i]);
        }
        p_test++;
    }
}

void initPlayers() {
    // init 2 empty boards to each player
    for (int p = 0; p < 2; p++) {
        for (int b = 0; b < 2; b++) {
            player[p][b] = initBoard();
        }
    }

    // init ship placement for both players
    int player_i = 0;
    char input[MAX_INPUT];
    while (player_i < 2) {
        printf("Player %d, please set up your ships (y, x, orientation)\n", player_i+1);
        for (int i = 0; i < NUM_OF_SHIPS; i++) {
            int success = false;
            while (1) {
                memset(input, 0, MAX_INPUT);
                printf("%s: ", ships[i]);
                fgets(input, MAX_INPUT, stdin);
                remove_white(input);
                success = placeShip(player[player_i][SHIPS], ships_cell_value[i], input);
                if (success == true) {
                    set_ship(player_i, i, input);
                    break;
                }
            }
        }
        if (player_i == 0) { printf("\n\n"); }
        player_i++;
    }
    printf("\nAll boats set up, Player 1 can now strike.\n\n");
}

// start at the ship coordinate and count all ship inputs. return false if none found. return true if at least one found
// input ship should be the index of which ship it references
int is_sunk(char** board, int ship, char coordinate[]) {
    // grid logic
    int row = coordinate[0] - 65;            // convert char into index via ascii
    int col = coordinate[1] - '0' - 1;       // convert char of a string into int
    char direction = coordinate[2];

    // traversing board
    if (direction == 'H') {
        for (int i = 0; i < ships_cell_value[ship]; i++) {
            // if ship already placed down
            if (getBoard(board, row, col+i) == '*') {
                return false;
            }
        }
    }
    else if (direction == 'V') {
        for (int i = 0; i < ships_cell_value[ship]; i++) {
            if (getBoard(board, row+i, col) == '*') {
                return false;
            }
        }
    }
    // returns true if ship has sunken
    printf("\nWe have sunk your %s!\n", ships[ship]);
    return true;
}

// input the enemies board here to check
int check_sunk(char** board, int enemy) {
    int sunk;
    int sunk_a_ship = false;
    for (int ship = 0; ship < NUM_OF_SHIPS; ship++) {
        if (sunk_ships[enemy][ship] == true) {
            continue;
        }
        sunk = is_sunk(board, ship, player_ship_coordinates[enemy][ship]);
        if (sunk == true) {
            sunk_ships[enemy][ship] = true;
            sunk_a_ship = true;
        }
    }
    return sunk_a_ship;
}

// takes in the player that's firing the shot
int fire(int player_firing, int row, int col) {
    // init player references
    int enemy;
    if (player_firing == 0) {
        enemy = 1;
    } else {
        enemy = 0;
    }

    // if has hit enemy
    if (getBoard(player[enemy][SHIPS], row, col) == '*') {
        setBoard(player[enemy][SHIPS], 'X',row, col);           // update enemy ships board
        setBoard(player[player_firing][SHOTS], 'X', row, col);  // update player firing's shots board
        int sunk = check_sunk(player[enemy][SHIPS], enemy);
        if (sunk == false) {
            printf("\nWe have hit the target!\n");
        }
    }
    else if (getBoard(player[enemy][SHIPS], row, col) == 'X') {
        printf("You have already fired at this location\n");
        return false;
    }
        // if missed enemy ships
    else {
        printf("\nYou have missed!\n");
        setBoard(player[player_firing][SHOTS], '0', row, col);  // update player firing's shots board
        setBoard(player[enemy][SHIPS], 'X', row, col);          // update player firing's shots board
    }
    return true;
}

int shots(int player_requesting) {
    toString(player[player_requesting][SHOTS]);
    return false;       // returning false because printing board doesn't qualify as a player's turn
}

int quit() {
    clean(player[0][SHIPS]);
    clean(player[0][SHOTS]);
    clean(player[1][SHIPS]);
    clean(player[1][SHOTS]);
    return -1;
}

// takes in which player is controlling, the command as a string and it's coordinates
int controls(int playerControlling, char* command, int row, int col) {
    if (strcmp(command, "fire") == 0) {
        return fire(playerControlling, row, col);
    }
    else if (strcmp(command, "shots") == 0) {
        return shots(playerControlling);
    }
}

void print_tokens(char* arr[]) {
    int count = 0;
    while (arr[count] != NULL) {
        printf("%s\n", arr[count]);
        count++;
    }
}

// tokenize user command
void format_input(char* tokens[], char user_in[]) {
    if (strcmp(user_in, "exit\n") == 0) {
        tokens[0] = "exit";
        return;
    }
    if (strcmp(user_in, "shots\n") == 0) {
        tokens[0] = "shots";
        return;
    }

    char* ptr;
    ptr = strtok(user_in," ");

    int counter = 0;
    while (counter < 3) {
        tokens[counter] = ptr;
        counter++;
        ptr = strtok(NULL, " ");
    }
//    print_tokens(tokens);
}

// update boards and contains game logic
int process_input(int player_inputting, char* tokens[]) {
    char* command = tokens[0];
    int size = 0, count = 0;
    while (tokens[count] != NULL) {
        size++;
        count++;
    }
    // base cases
    // if size == 1 check if right command
    if (size == 1) {
        if (strcmp(command, "exit") == 0) { return quit(); }
        if (strcmp(command, "shots") == 0) { return shots(player_inputting); }
        printf("\nUnrecognised command\n");
        return false;
    }
    // if input size doesn't equal 1 and doesn't equal 3
    if (size != 3) {
        printf("\nUnrecognised command\n");
        return false;
    }
    // size will always equal three if passed previous base cases
    if (strcmp(command, "fire") != 0) {
        printf("\nUnrecognised command\n");
        return false;
    }

    // grid logic
    // if number input is greater than 8 return false
    int col = atoi(tokens[2]);
    if (col > 8) {
        printf("Invalid coordinates\n");
        return false;
    }

    int row = tokens[1][0] - 65;            // convert string into char into index via ascii
    col = tokens[2][0] - '0' - 1;       // convert string into char of a string into int

    // if the input format is incorrect
    if ((row < 0) || (row > 7)) {
        printf("Invalid coordinates\n");
        return false;
    }
    if ((col < 0) || (col > 7)) {
        printf("Invalid coordinates\n");
        return false;
    }


    // success: parse input into player controls

    // return whether controls was successful or early exit
    return controls(player_inputting, command, row, col);
}

int playerLost(char** board) {
    int row, col;
    for (row = 0; row < ROW_MAX; row++) {
        for (col = 0; col < COL_MAX; col++) {
            if (board[row][col] == '*') {
                return false;
            }
        }
    }
    return true;
}

int gameFinished() {
    if (playerLost(player[0][SHIPS]) == true) {
        printf("\nPlayer 2 wins!\n");
        quit();
        return true;
    } else if (playerLost(player[1][SHIPS]) == true) {
        printf("\nPlayer 1 wins!\n");
        quit();
        return true;
    }
    return false;
}

void switch_player(int* player_inputting) {
    if (*player_inputting == 1) {
        *player_inputting = 0;
    } else {
        *player_inputting = 1;
    }
}

int playGame() {
    char input[MAX_INPUT];
    char* tokens[MAX_INPUT];

    // test code while(gameFinished )
    int player_inputting = 0;
    while (1) {
        memset(tokens, 0, sizeof(char*) * MAX_INPUT);

        fgets(input, MAX_INPUT, stdin);

//        printf("\nUser %d input: %s\n", player_inputting, input);

        format_input(tokens, input);
        int success = process_input(player_inputting, tokens);

        /* success determines which player's turn it is change on command
         * if success = true, then player switches
         * else if success = false, then player remains the same
         * else if success < 0, then game ends early and we clean mallocs and exit safely
         */

        // early exit
        if (success < 0) {
            break;
        }
        // successful input player changes
        if (success == true) {
            if (gameFinished() == true) {
                break;
            }
            switch_player(&player_inputting);
            printf("\nPlayer %d's turn\n\n", player_inputting+1);
        }
        // success false i.e. invalid input, will continue loop until input is right
    }
}

void initGame() {
    initPlayers();
    playGame();
}

int main() {
    initGame();
    return 0;
}