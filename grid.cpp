#include "grid.h"
#include "gamecontrol.h"

Grid::Grid(void *parent, int GridBlockSize, float StartX, float StartY) {
    m_gridBlockSize = GridBlockSize;
    m_gridXStart = StartX;
    m_gridYStart = StartY;

    m_parent = parent;
    Reset();
}

void Grid::Reset() {
    memset(&m_grid, '\0', sizeof(m_grid));
}

int Grid::GetGridBlockSize() {
    return m_gridBlockSize;
}

float Grid::GetGridXStart() {
    return m_gridXStart;
}

float Grid::GetGridYStart() {
    return m_gridYStart;
}

void *Grid::GetGrid() {
    return &m_grid;
}


//check to see if the placement of the tetromino will
//collide with any other blocks or go out of bounds
bool Grid::CheckCollision(Tetromino *tet) {
    char shape[4][4];
    memset(&shape, '\0', sizeof(shape));
    memcpy(&shape, tet->GetShape(), sizeof(shape));

    LOCATION_T location = tet->GetLocation();

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            //bounds check
            if (shape[y][x] == 1 && (
                y + location.GridY > GRID_HEIGHT - 1 ||
                y + location.GridY < 0))
                return true;
            //bounds check
            if (shape[y][x] == 1 && (
                x + location.GridX > GRID_WIDTH - 1 ||
                x + location.GridX < 0))
                return true;
            //collision check
            if (m_grid[y + location.GridY][x + location.GridX] == 1 && shape[y][x] == 1)
                return true;
        }
    }

    return false;
}

//This will take a shape and apply it to the grid.
//Collision detection must be done before this step and
//the tetromino supplied will become locked once it is complete.
void Grid::Merge(Tetromino *tet) {
    char shape[4][4];
    memset(&shape, '\0', sizeof(shape));
    memcpy(&shape, tet->GetShape(), sizeof(shape));

    LOCATION_T location = tet->GetLocation();

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shape[y][x] == 1)
                m_grid[y + location.GridY][x + location.GridX] = 1;
        }
    }
    tet->Lock();
    CheckCompleteLines();
}


//This will check for any completed lines
//It should be called only from the Merge() method
void Grid::CheckCompleteLines() {
    int line = 0;
    int numComplete = 0;
    for (int y = GRID_HEIGHT - 1; y > 0; y--) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (m_grid[y][x] == 1)
                line++;
        }
        if (line == GRID_WIDTH) {
            RemoveCompleteLine(y);
            y++;
            numComplete++;
        }
        line = 0;
    }

    if (numComplete > 0)
        ((GameControl*)m_parent)->AddCompleteLines(numComplete);
}

//this will cycle up the grid from the supplied row
//a complete row will be removed and the rows above will
//be dropped into its place.
void Grid::RemoveCompleteLine(int y) {
    for (int j = y; j > 0; j--) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            m_grid[j][x] = m_grid[j-1][x];
        }
    }
}
