#ifndef _TETROMINO_H_
#define _TETROMINO_H_

#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define I_SHAPE_ID 1
#define I_SHAPE_POS_1 1
#define I_SHAPE_POS_2 2

#define T_SHAPE_ID 2
#define T_SHAPE_POS_1 1
#define T_SHAPE_POS_2 2
#define T_SHAPE_POS_3 3
#define T_SHAPE_POS_4 4

#define O_SHAPE_ID 3
#define O_SHAPE_POS_1 1

#define L_SHAPE_ID 4
#define L_SHAPE_POS_1 1
#define L_SHAPE_POS_2 2
#define L_SHAPE_POS_3 3
#define L_SHAPE_POS_4 4

#define J_SHAPE_ID 5
#define J_SHAPE_POS_1 1
#define J_SHAPE_POS_2 2
#define J_SHAPE_POS_3 3
#define J_SHAPE_POS_4 4

#define S_SHAPE_ID 6
#define S_SHAPE_POS_1 1
#define S_SHAPE_POS_2 2

#define Z_SHAPE_ID 7
#define Z_SHAPE_POS_1 1
#define Z_SHAPE_POS_2 2

#define NUM_SHAPES 7

typedef struct {
    float x, y;
    int GridX, GridY;
} LOCATION_T;

class Tetromino {
    public:
        Tetromino();

        void Reset();
        void Create(int Type = 0, int Position = 0);
        void SetShape(int Type = 0, int Position = 0);
        void SetMaxPositions(int Num);
        void SetGridLocation(int GridX, int GridY);
        void Lock();

        int GetMaxPositions();
        int GetPosition();
        int GetType();
        void *GetShape();
        LOCATION_T GetLocation();
        bool IsLocked();

    private:
        int m_errCode;

        char m_tetromino[4][4];
        int m_position;
        int m_type;
        int m_maxPositions;
        int m_gridXPos;
        int m_gridYPos;

        float m_xPos;
        float m_yPos;
        float m_width;
        float m_height;
        bool m_locked;

};

#endif
