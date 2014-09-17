#ifndef GRID_H
#define GRID_H

#include <memory.h>
#include "tetromino.h"


#define GRID_BLOCK_SIZE 20
#define GRID_HEIGHT 20
#define GRID_WIDTH 10

class Grid {
    public:
        Grid(void *parent, int GridBlockSize = GRID_BLOCK_SIZE, float StartX = 10, float StartY = 10);

        void Reset();
        void Merge(Tetromino *tet);
        void CheckCompleteLines();
        void RemoveCompleteLine(int y);
        bool CheckCollision(Tetromino *tet);

        int GetGridBlockSize();
        float GetGridXStart();
        float GetGridYStart();
        void *GetGrid();

    private:
        char m_grid[GRID_HEIGHT][GRID_WIDTH];
        int m_gridBlockSize;
        float m_gridXStart;
        float m_gridYStart;
        void *m_parent;
};

#endif
