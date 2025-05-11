/**
 * @file maze.c
 * @author Wang Kongling
 * @brief Code for the maze game for COMP1921 Assignment 2
 * NOTE - You can remove or edit this file however you like - this is just a provided skeleton code
 * which may be useful to anyone who did not complete assignment 1.
 */

#include <stdio.h>
#include <stdlib.h>

 // defines for max and min permitted dimensions
#define MAX_DIM 100
#define MIN_DIM 5

// defines for the required autograder exit codes
#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

typedef struct __Coord
{
    int x;
    int y;
} coord;

typedef struct __Maze
{
    char** map;
    int height;
    int width;
    coord start;
    coord end;
} maze;

/**
 * @brief Initialise a maze object - allocate memory and set attributes
 *
 * @param this pointer to the maze to be initialised
 * @param height height to allocate
 * @param width width to allocate
 * @return int 0 on success, 1 on fail
 */
int create_maze(maze* this, int height, int width)
{
    this->height = height;
    this->width = width;

    // Allocate two-dimensional array memory
    this->map = (char**)malloc(height * sizeof(char*));
    if (!this->map)
        return 1;

    for (int i = 0; i < height; i++)
    {
        this->map[i] = (char*)malloc(width * sizeof(char));
        if (!this->map[i])
        {
            for (int j = 0; j < i; j++)
                free(this->map[j]);
            free(this->map);
            return 1;
        }
    }

    // Initialize the starting point and ending point to invalid values
    this->start.x = this->start.y = -1;
    this->end.x = this->end.y = -1;
    return 0;
}

/**
 * @brief Free the memory allocated to the maze struct
 *
 * @param this the pointer to the struct to free
 */
void free_maze(maze* this)
{
    if (this->map)
    {
        for (int i = 0; i < this->height; i++)
        {
            free(this->map[i]);
        }
        free(this->map);
    }
}

/**
 * @brief Validate and return the width of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid width (5-100)
 */
int get_width(FILE *file)
{
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE *file)
{
}

/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze *this, FILE *file)
{
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze *this, coord *player)
{
    // make sure we have a leading newline..
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            // decide whether player is on this spot or not
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
        // end each row with a newline.
        printf("\n");
    }
}

/**
 * @brief Validates and performs a movement in a given direction
 *
 * @param this Maze struct
 * @param player The player's current position
 * @param direction The desired direction to move in
 */
void move(maze *this, coord *player, char direction)
{
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze *this, coord *player)
{
}

int main()
{
    // check args

    // set up some useful variables (you can rename or remove these if you want)
    coord *player;
    maze *this_maze = malloc(sizeof(maze));
    FILE *f;

    // open and validate mazefile

    // read in mazefile to struct

    // maze game loop

    // win

    // return, free, exit
}