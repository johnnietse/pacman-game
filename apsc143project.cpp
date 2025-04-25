// APSC 143 Engineering Programming Project Starter Code
// Feel free to change anything about this file, but do not remove "#include "colours.h".

// Make sure to include all relevant libraries
#include <stdio.h>
#include "colours.h"
#include <stdlib.h>
#include <time.h>

void importMap(); //These a kind of unnecessary but good if you want to jump straight to a function
char gameplay();
void printMap();

//Defining constants
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'
#define ROWS 11
#define COLUMN 11

//Global variables
char mapArray[11][11]; //Size of array with outer walls
int pacX = 5, pacY = 5; //position of original pacman
int nextY, nextX; //next position of pacman

int G1x = 1, G1y = 1, G2x = 9, G2y = 9; //both initial ghost positions
char g1standingOn = '.', g2standingOn = '.'; //when ghost moves use these functions so dots don't erase

int winCheck() { // This function will check if all the dots have been collected
    //go through all positions in array
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (mapArray[i][j] == DOT) {
                return 0; // not win/ continue
            }
        }
    }
    return 1; // win
}
int colCheck(int x1, int y1, int x2, int y2) { //check if there is a collision with ghost and player
    if (x1 == x2 && y1 == y2) //check if both coordinates match
        return 1; //lose
    return 0; //not lose/ continue
}
int loseCheck() { // This function will check if pacman runs into the ghost
    if (colCheck(pacX, pacY, G1x, G1y) || colCheck(nextX, nextY, G1x, G1y) ||
        colCheck(pacX, pacY, G2x, G2y) || colCheck(nextX, nextY, G2x, G2y)) {
        return 1; // lose
    }
    return 0; //not lose +/ continue
}
int sightlineCheck(int gx, int gy, int dir) { //check if the ghost has a sightline with the player
    // 0 up, 1 down, 2 left, 3 right
    switch (dir) {
        case 0: // up
            for (int i = gy;; i--) { //start at ghost and go until it hits wall or PACMAN
                if (mapArray[i][gx] == WALL) //if hits wall stop looking
                    break;
                if (mapArray[i][gx] == PACMAN) //if hits PACMAN move
                    return 0; // return dir
            }
            break;
        case 1: // down
            for (int i = gy;; i++) {
                if (mapArray[i][gx] == WALL)
                    break;
                if (mapArray[i][gx] == PACMAN)
                    return 1;
            }
            break;
        case 2: // left
            for (int i = gx;; i--) {
                if (mapArray[gy][i] == WALL)
                    break;
                if (mapArray[gy][i] == PACMAN)
                    return 2;
            }
            break;
        case 3: // right
            for (int i = gx;; i++) {
                if (mapArray[gy][i] == WALL)
                    break;
                if (mapArray[gy][i] == PACMAN)
                    return 3;
            }
            break;
    }
    int random;
    while (1) { // go through until a valid random is found
        random = rand() % 4; // random number on range (0,3) [0, 1, 2, 3]
        switch (random) {
            case 0:
                if (mapArray[gy - 1][gx] == WALL) // if it runs into the wall choose a new random
                    continue; //continue means choose new random not necessarily the next case
                break; //accept this random function
            case 1:
                if (mapArray[gy + 1][gx] == WALL)
                    continue;
                break;
            case 2:
                if (mapArray[gy][gx - 1] == WALL)
                    continue;
                break;
            case 3:
                if (mapArray[gy][gx + 1] == WALL)
                    continue;
                break;
        }
        return random; //execute the function
    }
}
void ghostSearch() { // This function will make the ghost move when it can see PACMAN

    int dir; // 0 up, 1 down, 2 left, 3 right
    if (G1x == pacX) { //up or down
        if (G1y > pacY) { //if ghosts position is greater than pac position
            dir = 0; //go up
        } else {
            dir = 1; //other go down
        }
    }
    if (G1y == pacY) {
        if (G1x > pacX) {
            dir = 2;
        } else {
            dir = 3;
        }
    }
    int ghostMove = sightlineCheck(G1x, G1y, dir);

    switch (ghostMove) { //this switch case check all the conditions for the code block above
        // comments are not necessary for every line because code is recycled for slightly different cases
        case 0: //up
            if (mapArray[G1y - 1][G1x] == WALL) // if the next position of the ghost equals the wall break out of the function
                break;
            if (mapArray[G1y - 1][G1x] != PACMAN) // if it does not hit PACMAN update map with the dot
                mapArray[G1y][G1x] = g1standingOn;
            G1y--; //move ghost one position
            g1standingOn = mapArray[G1y][G1x]; //save what the ghost is going to stand on
            if (mapArray[G1y][G1x] != PACMAN) //place ghost back on map
                mapArray[G1y][G1x] = 'G';
            break;
        case 1: //down
            if (mapArray[G1y + 1][G1x] == WALL)
                break;
            if (mapArray[G1y + 1][G1x] != PACMAN)
                mapArray[G1y][G1x] = g1standingOn;
            G1y++;
            g1standingOn = mapArray[G1y][G1x];
            if (mapArray[G1y][G1x] != PACMAN)
                mapArray[G1y][G1x] = 'G';
            break;
        case 2: //left
            if (mapArray[G1y][G1x - 1] == WALL)
                break;
            if (mapArray[G1y][G1x - 1] != PACMAN)
                mapArray[G1y][G1x] = g1standingOn;
            G1x--;
            g1standingOn = mapArray[G1y][G1x];
            if (mapArray[G1y][G1x] != PACMAN)
                mapArray[G1y][G1x] = 'G';
            break;
        case 3: //right
            if (mapArray[G1y][G1x + 1] == WALL)
                break;
            if (mapArray[G1y][G1x + 1] != PACMAN)
                mapArray[G1y][G1x] = g1standingOn;
            G1x++;
            g1standingOn = mapArray[G1y][G1x];
            if (mapArray[G1y][G1x] != PACMAN)
                mapArray[G1y][G1x] = 'G';
            break;
    }
    /******************** G2 *********************/
//same functionality as previous code but for the second ghost, additional comments not necessary
    if (G2x == pacX) {
        if (G2y > pacY) {
            dir = 0;
        } else {
            dir = 1;
        }
    }
    if (G2y == pacY) {
        if (G2x > pacX) {
            dir = 2;
        } else {
            dir = 3;
        }
    }
    ghostMove = sightlineCheck(G2x, G2y, dir);
    switch (ghostMove) {
        case 0:
            if (mapArray[G2y - 1][G2x] == WALL)
                break;
            if (mapArray[G2y - 1][G2x] != PACMAN)
                mapArray[G2y][G2x] = g2standingOn;
            G2y--;
            g2standingOn = mapArray[G2y][G2x];
            if (mapArray[G2y][G2x] != PACMAN)
                mapArray[G2y][G2x] = 'G';
            break;
        case 1:
            if (mapArray[G2y + 1][G2x] == WALL)
                break;
            if (mapArray[G2y + 1][G2x] != PACMAN)
                mapArray[G2y][G2x] = g2standingOn;
            G2y++;
            g2standingOn = mapArray[G2y][G2x];
            if (mapArray[G2y][G2x] != PACMAN)
                mapArray[G2y][G2x] = 'G';
            break;
        case 2:
            if (mapArray[G2y][G2x - 1] == WALL)
                break;
            if (mapArray[G2y][G2x - 1] != PACMAN)
                mapArray[G2y][G2x] = g2standingOn;
            G2x--;
            g2standingOn = mapArray[G2y][G2x];
            if (mapArray[G2y][G2x] != PACMAN)
                mapArray[G2y][G2x] = 'G';
            break;
        case 3:
            if (mapArray[G2y][G2x + 1] == WALL)
                break;
            if (mapArray[G2y][G2x + 1] != PACMAN)
                mapArray[G2y][G2x] = g2standingOn;
            G2x++;
            g2standingOn = mapArray[G2y][G2x];
            if (mapArray[G2y][G2x] != PACMAN)
                mapArray[G2y][G2x] = 'G';
            break;
    }
}



void importMap() { //the assignment may have said use pointers, I didn't, oopsie. This is easier though, so..... ya :)
    // import file
    FILE *fp;
    fp = fopen("map.txt", "r");

    char mapArrayFile[9][9]; // create temp map array
    // scanf all chars in one line 9 times
    for (int row = 0; row < 9; row++) {
        fscanf(fp, "%c  %c  %c  %c  %c  %c  %c  %c  %c", &mapArrayFile[row][0],
               &mapArrayFile[row][1], &mapArrayFile[row][2], &mapArrayFile[row][3],
               &mapArrayFile[row][4], &mapArrayFile[row][5], &mapArrayFile[row][6],
               &mapArrayFile[row][7], &mapArrayFile[row][8]);
        getc(fp);
        getc(fp);
    }
    // close file
    fclose(fp);

    // go through every index in the temp array and put in global array
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMN; j++) {
            if (j == 0 || i == 0 || j == 10 || i == 10) {
                mapArray[i][j] = WALL;
            } else if (i == 5 && j == 5) {
                mapArray[i][j] = PACMAN;
            } 
            else {
                if (rand() % 5 == 0) {
                    mapArray[i][j] = WALL;
                } else {
                    mapArray[i][j] = DOT;
                }
            }
        }
    }
}
char gameplay() {
    while (1) { //run as long as the conditions are "true"
        if (loseCheck()) // lose
            return 'L';
        if (winCheck()) // win
            return 'W';

        nextX = pacX; //next x position equal to current position
        nextY = pacY; //next y position equal to current position
        char userInput;
        scanf("%c", &userInput); //scan for user input

        switch (userInput) {

            case UP:
                nextY = pacY - 1; //next position is equal to current position minus 1 position (in the array)
                break;

            case DOWN:
                nextY = pacY + 1;
                break;

            case LEFT:
                nextX = pacX - 1;
                break;

            case RIGHT:
                nextX = pacX + 1;
                break;
        }
        if (mapArray[nextY][nextX] != WALL && mapArray[nextY][nextX] != GHOST) {
            mapArray[pacY][pacX] = EMPTY;
            mapArray[nextY][nextX] = PACMAN;
            if (pacX != nextX || pacY != nextY) {
                ghostSearch();
                printMap();
            }
            pacX = nextX;
            pacY = nextY;
        }
    }
}
void printMap() {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (mapArray[i][j] == PACMAN) {
                colourChange(YELLOW);
            }
                //if wall colour will be blue
            else if (mapArray[i][j] == WALL) {
                colourChange(BLUE);
            }
                //if ghost colour will be pink
            else if (mapArray[i][j] == GHOST) {
                colourChange(PINK);
            }
                //if dot colour will be white
            else if (mapArray[i][j] == DOT) {
                colourChange(WHITE);
            }
            printf("%c  ",mapArray[i][j]);
        }
        printf("\n");
    }
}
int main() {
    srand(time(NULL));
    importMap();
    printMap();
    char status = gameplay();
    if (status == 'W')
        printf("Congratulations! You win! Press any key to exit the game\n");
    if (status == 'L')
        printf("Sorry, you lose. Press any number keys to exit the game\n");
    return 0;
}