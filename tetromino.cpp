#include "tetromino.h"
#include <gl/glfw.h>

struct I_SHAPE_T {
    char I_POS_1[4][4];
    char I_POS_2[4][4];
    const static int MaxPositions = 2;
} I_SHAPE =
    {1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0};

struct L_SHAPE_T {
    char L_POS_1[4][4];
    char L_POS_2[4][4];
    char L_POS_3[4][4];
    char L_POS_4[4][4];
    const static int MaxPositions = 4;
} L_SHAPE = {
    0, 0, 1, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,

    0, 0, 0, 0,
    1, 1, 1, 0,
    1, 0, 0, 0,
    0, 0, 0, 0,

    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0};

struct J_SHAPE_T {
    char J_POS_1[4][4];
    char J_POS_2[4][4];
    char J_POS_3[4][4];
    char J_POS_4[4][4];
    const static int MaxPositions = 4;
} J_SHAPE = {
    1, 0, 0, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,

    0, 0, 0, 0,
    1, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 0,

    0, 1, 0, 0,
    0, 1, 0, 0,
    1, 1, 0, 0,
    0, 0, 0, 0};

struct O_SHAPE_T {
    char O_POS_1[4][4];
    const static int MaxPositions = 1;
} O_SHAPE = {
    0, 0, 0, 0,
    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 0, 0, 0};

struct S_SHAPE_T {
    char S_POS_1[4][4];
    char S_POS_2[4][4];
    const static int MaxPositions = 2;
} S_SHAPE = {
    0, 1, 1, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 1, 0,
    0, 0, 0, 0};


struct Z_SHAPE_T {
    char Z_POS_1[4][4];
    char Z_POS_2[4][4];
    const static int MaxPositions = 2;
} Z_SHAPE = {
    1, 1, 0, 0,
    0, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 1, 0, 0,
    1, 1, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 0};

struct T_SHAPE_T {
    const char T_POS_1[4][4];
    const char T_POS_2[4][4];
    const char T_POS_3[4][4];
    const char T_POS_4[4][4];
    const static int MaxPositions = 4;
} T_SHAPE = {
    0, 1, 0, 0,
    1, 1, 1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,

    0, 1, 0, 0,
    0, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,

    0, 0, 0, 0,
    1, 1, 1, 0,
    0, 1, 0, 0,
    0, 0, 0, 0,

    0, 1, 0, 0,
    1, 1, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 0};


Tetromino::Tetromino() {
    Reset();
}

//Resets the Tetromino object to have no shape
//You'll need to call the SetShape() method again
void Tetromino::Reset() {
    m_locked = false;
    memset(&m_tetromino, '\0', sizeof(m_tetromino));
}

//Tell the Tetromino how many different positions it has
void Tetromino::SetMaxPositions(int Num) {
    m_maxPositions = Num;
}

//Get the number of different positions the current Tetromino has
int Tetromino::GetMaxPositions() {
    return m_maxPositions;
}

int Tetromino::GetPosition() {
    return m_position;
}

int Tetromino::GetType() {
    return m_type;
}

void *Tetromino::GetShape() {
    return &m_tetromino;
}

LOCATION_T Tetromino::GetLocation() {
    LOCATION_T location;
    location.x = m_xPos;
    location.y = m_yPos;
    location.GridX = m_gridXPos;
    location.GridY = m_gridYPos;

    return location;
}

void Tetromino::SetGridLocation(int GridX, int GridY) {
    m_gridXPos = GridX;
    m_gridYPos = GridY;
}

void Tetromino::Lock() {
    m_locked = true;
}

bool Tetromino::IsLocked() {
    return m_locked;
}

unsigned int  prev_time = 0;

int Random(int max) {
    int result;
    unsigned int cur_time;



    cur_time = (unsigned int)time(NULL);
    /*if (cur_time == prev_time) {
        cur_time = (unsigned int)glfwGetTime();
    }*/

    //printf("cur: %d prev: %d\n", cur_time, prev_time);
    srand(cur_time);
    prev_time = cur_time;

    result = rand() % max;
    return result;
}

/*
Sets the Shape and the Position of the Tetromino
The method takes two parameters.
Supplying 0 for Position will force a random selection
You will need to specify a specifc type as only the Create()
method supplies random types.
*/
void Tetromino::SetShape(int Type, int Position) {
    Reset();

    switch(Type) {
        case I_SHAPE_ID:
            m_maxPositions = I_SHAPE.MaxPositions;
            if (Position == 0)
                Position = Random(I_SHAPE.MaxPositions) + 1;
            switch (Position) {
                case I_SHAPE_POS_1:
                    memcpy(&m_tetromino, &I_SHAPE.I_POS_1, sizeof(I_SHAPE.I_POS_1));
                    break;
                case I_SHAPE_POS_2:
                    memcpy(&m_tetromino, &I_SHAPE.I_POS_2, sizeof(I_SHAPE.I_POS_2));
                    break;
                default:
                    break;
            }
            break;
        case T_SHAPE_ID:
            m_maxPositions = T_SHAPE.MaxPositions;
            if (Position == 0)
                Position = Random(T_SHAPE.MaxPositions) + 1;
            switch (Position) {
                case T_SHAPE_POS_1:
                    memcpy(&m_tetromino, &T_SHAPE.T_POS_1, sizeof(T_SHAPE.T_POS_1));
                    break;
                case T_SHAPE_POS_2:
                    memcpy(&m_tetromino, &T_SHAPE.T_POS_2, sizeof(T_SHAPE.T_POS_2));
                    break;
                case T_SHAPE_POS_3:
                    memcpy(&m_tetromino, &T_SHAPE.T_POS_3, sizeof(T_SHAPE.T_POS_3));
                    break;
                case T_SHAPE_POS_4:
                    memcpy(&m_tetromino, &T_SHAPE.T_POS_4, sizeof(T_SHAPE.T_POS_4));
                    break;
                default:
                    break;
            }
            break;
        case O_SHAPE_ID:
            m_maxPositions = O_SHAPE.MaxPositions;
            if (Position == 0)
                Position = Random(O_SHAPE.MaxPositions) + 1;
            switch (Position) {
                case O_SHAPE_POS_1:
                    memcpy(&m_tetromino, &O_SHAPE.O_POS_1, sizeof(O_SHAPE.O_POS_1));
                    break;
                default:
                    break;
            }
            break;
        case L_SHAPE_ID:
            m_maxPositions = L_SHAPE.MaxPositions;
            if (Position == 0)
                Position = Random(L_SHAPE.MaxPositions) + 1;
            switch (Position) {
                case L_SHAPE_POS_1:
                    memcpy(&m_tetromino, &L_SHAPE.L_POS_1, sizeof(L_SHAPE.L_POS_1));
                    break;
                case L_SHAPE_POS_2:
                    memcpy(&m_tetromino, &L_SHAPE.L_POS_2, sizeof(L_SHAPE.L_POS_2));
                    break;
                case L_SHAPE_POS_3:
                    memcpy(&m_tetromino, &L_SHAPE.L_POS_3, sizeof(L_SHAPE.L_POS_3));
                    break;
                case L_SHAPE_POS_4:
                    memcpy(&m_tetromino, &L_SHAPE.L_POS_4, sizeof(L_SHAPE.L_POS_4));
                    break;
                default:
                    break;
            }
            break;
        case J_SHAPE_ID:
            m_maxPositions = J_SHAPE.MaxPositions;
            if (Position == 0)
                Position = Random(J_SHAPE.MaxPositions) + 1;
            switch (Position) {
                case J_SHAPE_POS_1:
                    memcpy(&m_tetromino, &J_SHAPE.J_POS_1, sizeof(J_SHAPE.J_POS_1));
                    break;
                case J_SHAPE_POS_2:
                    memcpy(&m_tetromino, &J_SHAPE.J_POS_2, sizeof(J_SHAPE.J_POS_2));
                    break;
                case J_SHAPE_POS_3:
                    memcpy(&m_tetromino, &J_SHAPE.J_POS_3, sizeof(J_SHAPE.J_POS_3));
                    break;
                case J_SHAPE_POS_4:
                    memcpy(&m_tetromino, &J_SHAPE.J_POS_4, sizeof(J_SHAPE.J_POS_4));
                    break;
                default:
                    break;
            }
            break;
        case S_SHAPE_ID:
            m_maxPositions = S_SHAPE.MaxPositions;
            if (Position == 0)
                Position = Random(S_SHAPE.MaxPositions) + 1;
            switch (Position) {
                case S_SHAPE_POS_1:
                    memcpy(&m_tetromino, &S_SHAPE.S_POS_1, sizeof(S_SHAPE.S_POS_1));
                    break;
                case S_SHAPE_POS_2:
                    memcpy(&m_tetromino, &S_SHAPE.S_POS_2, sizeof(S_SHAPE.S_POS_2));
                    break;
                default:
                    break;
            }
            break;
        case Z_SHAPE_ID:
            m_maxPositions = Z_SHAPE.MaxPositions;
            if (Position == 0)
                Position = Random(Z_SHAPE.MaxPositions) + 1;
            switch (Position) {
                case Z_SHAPE_POS_1:
                    memcpy(&m_tetromino, &Z_SHAPE.Z_POS_1, sizeof(Z_SHAPE.Z_POS_1));
                    break;
                case Z_SHAPE_POS_2:
                    memcpy(&m_tetromino, &Z_SHAPE.Z_POS_2, sizeof(Z_SHAPE.Z_POS_2));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    m_type = Type;
    m_position = Position;
}

/*
    Type - shape of the tetromino. If set to 0 will force randomisation.
    Position - position of the tetromino. If set to 0 will force randomisation.
*/
void Tetromino::Create(int Type, int Position) {
    if (Type == 0) {
        Type = Random(NUM_SHAPES) + 1;
    }

    switch (Type) {
        case I_SHAPE_ID:
            SetGridLocation(3, 0);
            SetShape(I_SHAPE_ID);
            break;
        case T_SHAPE_ID:
            SetGridLocation(4, 0);
            SetShape(T_SHAPE_ID);
            break;
        case O_SHAPE_ID:
            SetGridLocation(4, 0);
            SetShape(O_SHAPE_ID);
            break;
        case L_SHAPE_ID:
            SetGridLocation(4, 0);
            SetShape(L_SHAPE_ID);
            break;
        case J_SHAPE_ID:
            SetGridLocation(4, 0);
            SetShape(J_SHAPE_ID);
            break;
        case S_SHAPE_ID:
            SetGridLocation(4, 0);
            SetShape(S_SHAPE_ID);
            break;
        case Z_SHAPE_ID:
            SetGridLocation(4, 0);
            SetShape(Z_SHAPE_ID);
            break;
        default:
            break;
    }
    m_type = Type;
}

