#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include <gl/glfw.h>
#include <string.h>
#include "tetromino.h"
#include "grid.h"



#define LINES_PER_LEVEL 10
#define DROP_INTERVAL_DEFAULT 1.0
#define DROP_INTERVAL_INCREMENT 0.049
#define DROP_ACCEL_DEFAULT 0

#define BLOCK_TEXTURE   0
#define BG_TEXTURE      1
#define FONT_TEXTURE    2
#define MAIN_BG_TEXTURE 3

#define NUM_TEXTURES    4

class GameControl {
    public:
        GameControl(double DropInterval = DROP_INTERVAL_DEFAULT, float DropAccel = DROP_ACCEL_DEFAULT);
        ~GameControl();

        void SetDropInterval(double Interval);
        void SetDropAccel(float DropAccel);
        void AddCompleteLines(int Lines);
        void AddScore(long Score);
        void SetTetromino(Tetromino *tetromino);
        void SetGrid(Grid *grid);
        void SetTextureBase(int base);
        void SetLevel(int Level);
        void SetPaused(bool Paused);
        void SetFPS(double FPS);
        void StartNewGame();
        void SetGameState(bool Running);

        void CaptureKeys(bool Capture);

        double GetOldTime();

        double GetFPS();
        float GetDropAccel();
        double GetDropInterval();
        int GetCompleteLines();
        long GetScore();
        int GetLevel();
        Tetromino *GetTetromino();
        Tetromino *GetNextTetromino();
        Grid *GetGrid();
        GLuint *GetTextures();
        GLuint GetTextureBase();
        bool GetPaused();
        bool IsGameRunning();

        void GameKeyHandle(int key, int action);

    private:
        bool m_running;
        bool m_paused;

        float m_dropAccel;
        double m_dropInterval;
        double m_fps;

        int m_completeLines, m_prevLines;
        int m_level;
        long m_score;
        long m_playingTime;

        Tetromino *m_tetromino;
        Tetromino *m_nextTetromino;
        Grid *m_grid;

        GLuint m_texBase;
        GLuint m_textures[NUM_TEXTURES];
};

bool SetupScene(int texBase, GLuint *textures);
void GameLoop();
void UpdateDisplay();
void GLFWCALL KeyHandle(int key, int action);

#endif
