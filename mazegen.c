#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MIN_DIM 5
#define MAX_DIM 100

void generate_maze(const char* filename, int width, int height);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <Filename> <width> <height>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Width and height
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    if (width < MIN_DIM || width > MAX_DIM ||
        height < MIN_DIM || height > MAX_DIM) {
        fprintf(stderr, "Size must between %d and %d\n", MIN_DIM, MAX_DIM);
        return EXIT_FAILURE;
    }

    // Generate a maze and save it to a file
    generate_maze(argv[1], width, height);
    return EXIT_SUCCESS;
}


typedef struct {
    int x, y;
} Cell;

// Walls and paths
typedef enum {
    WALL,
    PATH
} Tile;

typedef struct {
    Tile** grid;    // Maze grid
    int width;      // Actual width
    int height;     // Actual height
} Maze;


Maze* create_maze(int width, int height) {
    Maze* maze = malloc(sizeof(Maze));
    maze->width = 2 * width + 1;   // The width of the wall
    maze->height = 2 * height + 1; // The height of the wall
    maze->grid = (Tile**)malloc(maze->height * sizeof(Tile*));
    for (int i = 0; i < maze->height; i++) {
        maze->grid[i] = (Tile*)calloc(maze->width, sizeof(Tile));
        // Wall initialization
        for (int j = 0; j < maze->width; j++) {
            maze->grid[i][j] = WALL;
        }
    }
    return maze;
}


void generate_with_dfs(Maze* maze) {
    Cell stack[10000]; // Stack size
    int top = 0;
    int dx[] = { 0, 0, 2, -2 };
    int dy[] = { 2, -2, 0, 0 };

    Cell start = { 1, 1 };
    maze->grid[start.y][start.x] = PATH;
    stack[top++] = start;

    srand(time(NULL));

    while (top > 0) {
        Cell current = stack[--top];
        int dirs[4] = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; i++) {
            int j = rand() % 4;
            int temp = dirs[i];
            dirs[i] = dirs[j];
            dirs[j] = temp;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[dirs[i]];
            int ny = current.y + dy[dirs[i]];
            if (nx >= 1 && nx < maze->width - 1 &&
                ny >= 1 && ny < maze->height - 1 &&
                maze->grid[ny][nx] == WALL) {
                // Break through the wall
                maze->grid[ny - dy[dirs[i]] / 2][nx - dx[dirs[i]] / 2] = PATH;
                maze->grid[ny][nx] = PATH;
                stack[top++] = (Cell){ nx, ny };
            }
        }
    }
}


void set_start_end(Maze* maze) {
    // The starting point is in the upper left part.
    maze->grid[1][1] = 'S';
    // The end point is in the lower right corner part.
    maze->grid[maze->height - 2][maze->width - 2] = 'E';
}


void save_maze(Maze* maze, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Unable to create file");
        exit(EXIT_FAILURE);
    }

    // Convert Tile to characters
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            switch (maze->grid[i][j]) {
            case WALL:  fputc('#', file); break;
            case PATH:  fputc(' ', file); break;
            case 'S':   fputc('S', file); break;
            case 'E':   fputc('E', file); break;
            }
        }
        fputc('\n', file);
    }
    fclose(file);
}


void generate_maze(const char* filename, int width, int height) {
    Maze* maze = create_maze(width, height);
    generate_with_dfs(maze);
    set_start_end(maze);
    save_maze(maze, filename);
    // Release memory
    for (int i = 0; i < maze->height; i++) free(maze->grid[i]);
    free(maze->grid);
    free(maze);
}