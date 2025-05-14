/**
 * @file maze.c
 * @author Wang Kongling
 * @brief Code for the maze game for COMP1921 Assignment 2
 * NOTE - You can remove or edit this file however you like - this is just a provided skeleton code
 * which may be useful to anyone who did not complete assignment 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int get_width(FILE* file) {
    char buffer[MAX_DIM + 2];
    int width = 0;

    // Read the first line to determine the width
    if (fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0'; // 去除换行符
            len--;
        }
        width = len;
    }

    // Verify the width
    if (width < MIN_DIM || width > MAX_DIM) return 0;

    // Reset the file pointer
    rewind(file);
    return width;
}

/**
 * @brief Validate and return the height of the mazefile
 *
 * @param file the file pointer to check
 * @return int 0 for error, or a valid height (5-100)
 */
int get_height(FILE* file) {
    char buffer[MAX_DIM + 2];
    int height = 0;

    // Count the number of valid lines
    while (fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        if (len > 0) height++; // Ignore blank lines
    }

    // Verify the height
    if (height < MIN_DIM || height > MAX_DIM) return 0;

    // Reset the file pointer
    rewind(file);
    return height;
}
/**
 * @brief read in a maze file into a struct
 *
 * @param this Maze struct to be used
 * @param file Maze file pointer
 * @return int 0 on success, 1 on fail
 */
int read_maze(maze* this, FILE* file)
{
    char buffer[MAX_DIM + 2]; // +2Consider line breaks and null characters

    for (int i = 0; i < this->height; i++)
    {
        if (!fgets(buffer, sizeof(buffer), file))
        {
            return 1; //read failure
        }

        // Processing line breaks and calculating length
        int len = 0;
        int has_newline = 0;
        while (len < (MAX_DIM + 2) && buffer[len] != '\0')
        {
            if (buffer[len] == '\n')
            {
                has_newline = 1;
                break;
            }
            len++;
        }
        if (has_newline)
        {
            buffer[len] = '\0';
        }

        // Verify line length
        if (len != this->width)
        {
            return 1;
        }

        for (int j = 0; j < this->width; j++)
        {
            char c = buffer[j];
            this->map[i][j] = c;

            // Record the starting and ending points
            if (c == 'S')
            {
                if (this->start.x != -1)
                    return 1; // Multiple starting points
                this->start.x = j;
                this->start.y = i;
            }
            else if (c == 'E')
            {
                if (this->end.x != -1)
                    return 1; // Multiple endpoints
                this->end.x = j;
                this->end.y = i;
            }
            else if (c != '#' && c != ' ')
            {
                return 1;
            }
        }
    }

    // Verify the existence of the starting point and the ending point
    if (this->start.x == -1 || this->end.x == -1)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Prints the maze out - code provided to ensure correct formatting
 *
 * @param this pointer to maze to print
 * @param player the current player location
 */
void print_maze(maze* this, coord* player)
{
    printf("\n");
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            if (player->x == j && player->y == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", this->map[i][j]);
            }
        }
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
void move(maze* this, coord* player, char direction)
{
    int dx = 0, dy = 0;

    // Handle case-sensitive input
    switch (direction)
    {
    case 'W':
    case 'w':
        dy = -1;
        break;
    case 'A':
    case 'a':
        dx = -1;
        break;
    case 'S':
    case 's':
        dy = 1;
        break;
    case 'D':
    case 'd':
        dx = 1;
        break;
    default:
        return;
    }

    // Calculate the new position
    int new_x = player->x + dx;
    int new_y = player->y + dy;

    // Boundary and wall inspection
    if (new_x >= 0 && new_x < this->width &&
        new_y >= 0 && new_y < this->height &&
        this->map[new_y][new_x] != '#')
    {
        player->x = new_x;
        player->y = new_y;
    }
}

/**
 * @brief Check whether the player has won and return a pseudo-boolean
 *
 * @param this current maze
 * @param player player position
 * @return int 0 for false, 1 for true
 */
int has_won(maze* this, coord* player)
{
    return (player->x == this->end.x) &&
        (player->y == this->end.y);
}

int main(int argc, char* argv[])
{
    // Parameter validation
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Maze file>\n", argv[0]);
        exit(EXIT_ARG_ERROR);
    }

    // Operate files
    FILE* f = fopen(argv[1], "r");
    if (!f)
    {
        fprintf(stderr, "can not open file\n");
        exit(EXIT_FILE_ERROR);
    }

    // Read the maze size
    int width = get_width(f);
    int height = get_height(f);
    if (width == 0 || height == 0)
    {
        fclose(f);
        fprintf(stderr, "Invalid maze size\n");
        exit(EXIT_MAZE_ERROR);
    }

    //Initialize the maze structure
    maze* this_maze = (maze*)malloc(sizeof(maze));
    if (create_maze(this_maze, height, width))
    {
        fclose(f);
        free(this_maze);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_MAZE_ERROR);
    }

    // Read maze data
    if (read_maze(this_maze, f))
    {
        fclose(f);
        free_maze(this_maze);
        free(this_maze);
        fprintf(stderr, "The maze format is incorrect\n");
        exit(EXIT_MAZE_ERROR);
    }
    fclose(f);

    // Loop game
    char input[10];
    coord player = this_maze->start;
    while (!has_won(this_maze, &player))
    {
        print_maze(this_maze, &player);
        printf("Enter the moving direction (W/A/S/D): ");

        if (!fgets(input, sizeof(input), stdin))
        {
            break;
        }

        move(this_maze, &player, input[0]);
    }

    // End processing
    printf("\nCongratulations You've successfully made your way out of the maze\n");
    free_maze(this_maze);
    free(this_maze);
    return EXIT_SUCCESS;
}