#include "gamecontrol.h"
#include "menucontrol.h"
#include "soundcontrol.h"

GameControl *game;
extern MenuControl *menu;
extern SoundControl *sound;

GameControl::GameControl(double DropInterval, float DropAccel) {
    m_dropInterval = 1.0f;
    m_dropAccel = 0.0f;
    m_completeLines = m_prevLines = 0;
    m_level = 0;
    m_score = 0;
    m_paused = false;
    //m_oldtime = m_newtime = m_deltatime = m_lastDropTime = 0;

    m_tetromino = new Tetromino();
    m_nextTetromino = new Tetromino();
    m_tetromino->Create();

    m_grid = new Grid(this, GRID_BLOCK_SIZE, 200, 20);

    SetDropInterval(GetDropInterval() - (m_level * DROP_INTERVAL_INCREMENT));
}

GameControl::~GameControl() {
    if (glfwGetWindowParam(GLFW_OPENED)) {
        glDeleteTextures(NUM_TEXTURES, m_textures);
    }

    delete m_grid;
    delete m_tetromino;

}

void GameControl::SetDropAccel(float DropAccel = 0.0f) {
    m_dropAccel = DropAccel;
}

float GameControl::GetDropAccel() {
    return m_dropAccel;
}

void GameControl::SetDropInterval(double Interval) {
    m_dropInterval = Interval;
}

double GameControl::GetDropInterval() {
    return m_dropInterval;
}

int GameControl::GetLevel() {
    return m_level;
}

void GameControl::AddCompleteLines(int Lines) {
    m_completeLines += Lines;
    int diffLines = m_completeLines - m_prevLines;

    switch (diffLines) {
        case 1: //single line clear
            AddScore((m_level+1)*40);
            break;
        case 2: //double line clear
            AddScore((m_level+1)*100);
            break;
        case 3: //triple line clear
            AddScore((m_level+1)*300);
            break;
        case 4: //tetris line clear
            AddScore((m_level+1)*1200);
            break;
        default:
            break;
    }

    //increase the level if we've got enough lines and then
    //speed up the drop interval
    m_prevLines = m_completeLines;
    int currentLevel;
    currentLevel = int(m_completeLines / LINES_PER_LEVEL);
    if (currentLevel > GetLevel()) {
        m_level++;
        SetDropInterval(GetDropInterval() - (DROP_INTERVAL_INCREMENT));
        sound->Play(SOUND_LEVEL_UP);
    }
    else
        sound->Play(SOUND_CLEAR_LINE);
}

int GameControl::GetCompleteLines() {
    return m_completeLines;
}

//adds the specified amount to the total score
void GameControl::AddScore(long Score) {
    m_score += Score;
}

long GameControl::GetScore() {
    return m_score;
}

void GameControl::SetTetromino(Tetromino *tetromino) {
    memcpy(m_tetromino, tetromino, sizeof(Tetromino));
    //m_tetromino = tetromino;
}

Tetromino *GameControl::GetTetromino() {
    return m_tetromino;
}

Tetromino *GameControl::GetNextTetromino() {
    return m_nextTetromino;
}

void GameControl::SetGrid(Grid *grid) {
    m_grid = grid;
}

Grid *GameControl::GetGrid() {
    return m_grid;
}

GLuint *GameControl::GetTextures() {
    return m_textures;
}

void GameControl::SetTextureBase(int base) {
    m_texBase = base;
}

GLuint GameControl::GetTextureBase() {
    return m_texBase;
}

void GameControl::CaptureKeys(bool Capture) {
    if (Capture) {
        glfwEnable(GLFW_STICKY_KEYS);
        glfwSetKeyCallback(KeyHandle);
    }
    else {
        glfwDisable(GLFW_STICKY_KEYS);
        glfwSetKeyCallback(NULL);
    }
}

void GameControl::SetLevel(int Level) {
    m_level = Level;
}

void GameControl::SetPaused(bool Paused) {
    m_paused = Paused;
}

bool GameControl::GetPaused() {
    return m_paused;
}

void GameControl::StartNewGame() {
    m_score = 0;
    m_completeLines = 0;
    //reset everything for the next game
    game->GetTetromino()->Reset();

    //what a hack... its the best i can think of to get
    //something slightly random :/
    game->SetTetromino(game->GetNextTetromino());

    game->GetNextTetromino()->Reset();
    game->GetNextTetromino()->Create();

    game->CaptureKeys(true);
    game->GetGrid()->Reset();
    game->SetLevel(0);
    game->SetDropInterval(DROP_INTERVAL_DEFAULT - (game->GetLevel() * DROP_INTERVAL_INCREMENT));
}

void GameControl::SetGameState(bool Running) {
    m_running = Running;
}

bool GameControl::IsGameRunning() {
    return m_running;
}

void GameControl::SetFPS(double FPS) {
    m_fps = FPS;
}

double GameControl::GetFPS() {
    return m_fps;
}

void MoveDown(Grid *grid, Tetromino *tetromino) {
    LOCATION_T location = tetromino->GetLocation();
    Tetromino *temp = new Tetromino();
    memcpy(temp, tetromino, sizeof(Tetromino));

    temp->SetGridLocation(location.GridX, location.GridY + 1);
    if (!grid->CheckCollision(temp)) {
        tetromino->SetGridLocation(location.GridX, location.GridY + 1);
    }
    else {
        grid->Merge(tetromino);
    }

    delete temp;
}

void DrawTetromino(Grid *grid, Tetromino *tetromino, GLuint *textures) {
    char shape[4][4];
    memset(&shape, '\0', sizeof(shape));
    memcpy(&shape, tetromino->GetShape(), sizeof(shape));

    int blocksize = grid->GetGridBlockSize();
    float startx = grid->GetGridXStart();
    float starty = grid->GetGridYStart();

    LOCATION_T location = tetromino->GetLocation();
    startx += (location.GridX * blocksize);
    starty += (location.GridY * blocksize);

    glBindTexture(GL_TEXTURE_2D, textures[BLOCK_TEXTURE]);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shape[y][x] == 1) {
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex2d(startx + (x * blocksize), starty + (y * blocksize));
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex2d(startx + (x * blocksize) + blocksize, starty + (y * blocksize));
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex2d(startx + (x * blocksize) + blocksize, starty + (y * blocksize) + blocksize);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex2d(startx + (x * blocksize), starty + (y * blocksize) + blocksize);
                glEnd();
            }
        }
    }
}

void DrawPreview() {
    char shape[4][4];
    memset(&shape, '\0', sizeof(shape));
    memcpy(&shape, game->GetNextTetromino()->GetShape(), sizeof(shape));

    Grid *grid = game->GetGrid();
    int blocksize = grid->GetGridBlockSize();
    float startx, starty;
    startx = grid->GetGridXStart() + (GRID_WIDTH * blocksize) + 40;
    starty = grid->GetGridYStart();

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(startx, starty);
        glVertex2f(startx + (4 * blocksize) + 20, starty);
        glVertex2f(startx + (4 * blocksize) + 20, starty + (5 * blocksize));
        glVertex2f(startx, starty + (5 * blocksize));
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPopMatrix();


    startx += 10;
    starty += 10;

    glBindTexture(GL_TEXTURE_2D, game->GetTextures()[BLOCK_TEXTURE]);
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (shape[y][x] == 1) {
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex2d(startx + (x * blocksize), starty + (y * blocksize));
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex2d(startx + (x * blocksize) + blocksize, starty + (y * blocksize));
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex2d(startx + (x * blocksize) + blocksize, starty + (y * blocksize) + blocksize);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex2d(startx + (x * blocksize), starty + (y * blocksize) + blocksize);
                glEnd();
            }
        }
    }
}


void DrawGrid(Grid *grid, GLuint *textures) {
    int blocksize = grid->GetGridBlockSize();
    float startx = grid->GetGridXStart();
    float starty = grid->GetGridYStart();

    //Blend with the background image
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

    //draw background texture
    glBindTexture(GL_TEXTURE_2D, textures[BG_TEXTURE]);
    glColor4f(0.7f, 0.7f, 0.7f, 0.2f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(startx, starty);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(startx + (GRID_WIDTH * blocksize), starty);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(startx + (GRID_WIDTH * blocksize), starty + (GRID_HEIGHT * blocksize));
        glTexCoord2f(0.f, 1.0f);
        glVertex2f(startx, starty + (GRID_HEIGHT * blocksize));
    glEnd();

    //draw grid lines
    glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
    //glEnable(GL_LINE_SMOOTH);
    for (int y = 0; y < GRID_HEIGHT + 1; y++) {
        glBegin(GL_LINE_STRIP);
            glVertex2f(startx, starty + (y * blocksize));
            glVertex2f(startx + (GRID_WIDTH * blocksize), starty + (y * blocksize));
        glEnd();
    }

    for (int x = 0; x < GRID_WIDTH + 1; x++) {
        glBegin(GL_LINE_STRIP);
            glVertex2f(startx + (x * blocksize), starty);
            glVertex2f(startx + (x * blocksize), starty + (GRID_HEIGHT * blocksize));
        glEnd();
    }

    glDisable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    //draw merged blocks
    glBindTexture(GL_TEXTURE_2D, textures[BLOCK_TEXTURE]);
    char shape[GRID_HEIGHT][GRID_WIDTH];
    memset(&shape, '\0', sizeof(shape));
    memcpy(&shape, grid->GetGrid(), sizeof(shape));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (shape[y][x] == 1) {
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex2d(startx + (x * blocksize), starty + (y * blocksize));
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex2d(startx + (x * blocksize) + blocksize, starty + (y * blocksize));
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex2d(startx + (x * blocksize) + blocksize, starty + (y * blocksize) + blocksize);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex2d(startx + (x * blocksize), starty + (y * blocksize) + blocksize);
                glEnd();
            }
        }
    }
}

void GenerateFont(int base, GLuint *textures) {
    //thanks to NEHE for this font stuff

    //float csx = 0.078125f;
    float csx = 0.0625f;
    float csy = 0.0625f;


    game->SetTextureBase(glGenLists(256));
    glBindTexture(GL_TEXTURE_2D, game->GetTextures()[FONT_TEXTURE]);

    for (int charnum = 0; charnum  < 256; charnum++) {
        float cx = float(charnum % 16) / 16.0f;
        float cy = float(charnum / 16) / 16.0f;

        glNewList(game->GetTextureBase()+charnum, GL_COMPILE);


            glBegin(GL_QUADS);
                //glTexCoord2f(0.0f, 1-0.0625f);
                glTexCoord2f(cx, 1-cy-csy);
                glVertex2i(0, 16);

                //glTexCoord2f(0.0625f, 1-0.0625f);
                glTexCoord2f(cx+csx, 1-cy-csy);
                glVertex2i(16, 16);

                //glTexCoord2f(0.0625f, 1.0f);
                glTexCoord2f(cx+csx, 1.0f-cy);
                glVertex2i(16, 0);

                //glTexCoord2f(0.0f, 1.0f);
                glTexCoord2f(cx, 1.0f-cy);
                glVertex2i(0, 0);
            glEnd();

            glTranslated(16, 0, 0);

        glEndList();
    }
}

//draws the text for the scores etc
void DrawText() {
    //TODO: ugly ugly ugly!!! fix fix fix the DrawText()
    int scoreStartX = (game->GetGrid()->GetGridXStart() + (game->GetGrid()->GetGridBlockSize() * GRID_WIDTH) + 50);
    int scoreStartY = 150;

    //save where we are
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);

    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    //draw the blended box for the score details
    glBegin(GL_QUADS);
        glVertex2i(scoreStartX - 16, scoreStartY - 5);
        glVertex2i(scoreStartX + 96, scoreStartY - 5);
        glVertex2i(scoreStartX + 96, scoreStartY + (32 * 3) + 5);
        glVertex2i(scoreStartX - 16, scoreStartY + (32 * 3) + 5);
    glEnd();

    glPopMatrix();
    glEnable(GL_TEXTURE_2D);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    //removes the black background in the font
    glBlendFunc(GL_ONE, GL_DST_ALPHA);

    glListBase(game->GetTextureBase()-32);
    glBindTexture(GL_TEXTURE_2D, game->GetTextures()[FONT_TEXTURE]);

    glPushMatrix();

    //FPS
    glTranslated(0,0,0);
    char fps [sizeof(int)*8+1];
    int sign, decimal;
    itoa(game->GetFPS(), fps, 10);

    glCallLists(strlen(fps), GL_UNSIGNED_BYTE, fps);

    glPopMatrix();

    glTranslated(scoreStartX, scoreStartY, 0);
    glPushMatrix();

    char score [sizeof(long)*8+1];
    ltoa(game->GetScore(), score, 10);

    //Score
    glCallLists(5, GL_UNSIGNED_BYTE, "SCORE");
    glTranslated((-(6 * 16) / 2) - float((strlen(score) - 1) * 16 / 2), 16 , 0);
    glCallLists(strlen(score), GL_UNSIGNED_BYTE, &score);

    glPopMatrix();

    //Level
    glTranslated(0, 32, 0);
    glPushMatrix();

    char level[sizeof(int) * 8 + 1];
    itoa(game->GetLevel(), level, 10);

    glCallLists(5, GL_UNSIGNED_BYTE, "LEVEL");
    glTranslated((-(6 * 16) / 2) - float((strlen(level) - 1) * 16 / 2), 16 , 0);

    glCallLists(strlen(level), GL_UNSIGNED_BYTE, &level);

    //Lines
    glPopMatrix();
    glTranslated(0, 32, 0);

    char lines[sizeof(int) * 8 + 1];
    itoa(game->GetCompleteLines(), lines, 10);

    glCallLists(5, GL_UNSIGNED_BYTE, "LINES");
    glTranslated((-(6 * 16) / 2) - float((strlen(lines) - 1) * 16 / 2), 16 , 0);

    glCallLists(strlen(lines), GL_UNSIGNED_BYTE, &lines);

    glDisable(GL_BLEND);

    //go back where we were
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

bool SetupScene(int texBase, GLuint *textures) {
    if (!glfwInit()) {
        printf("glfwInit() failed\n");
        return false;
    }

    if( !glfwOpenWindow(640, 480, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
    {
        printf("glfwOpenWindow() failed\n");
        return false;
    }

    glfwSetWindowTitle("Heavy Metal");

    glGenTextures(NUM_TEXTURES, textures);
    glBindTexture(GL_TEXTURE_2D, textures[BLOCK_TEXTURE]);

    if (!glfwLoadTexture2D("block.tga", GLFW_BUILD_MIPMAPS_BIT)) {
        printf("Texture load failed.\n");
        //return false;
    }
    else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                     GL_LINEAR );
    }

    glBindTexture(GL_TEXTURE_2D, textures[BG_TEXTURE]);
    if (!glfwLoadTexture2D("bg.tga", GLFW_BUILD_MIPMAPS_BIT)) {
        printf("Texture load failed.\n");
        //return false;
    }
    else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR );

    }

    glBindTexture(GL_TEXTURE_2D, textures[FONT_TEXTURE]);
    if (!glfwLoadTexture2D("font.tga", GLFW_BUILD_MIPMAPS_BIT)) {
        printf("Texture load failed.\n");
    }
    else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR );
    }

    glBindTexture(GL_TEXTURE_2D, textures[MAIN_BG_TEXTURE]);
    if (!glfwLoadTexture2D("main_bg.tga", GLFW_BUILD_MIPMAPS_BIT)) {
        printf("Texture load failed.\n");
    }
    else {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR );
    }

    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, 640, 480);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);							    // The Type Of Depth Test To Do
	glShadeModel(GL_SMOOTH);

    /*glCullFace( GL_FRONT );
    glEnable( GL_CULL_FACE );*/

    //load the font into the calllists for later use
    GenerateFont(texBase, textures);

    game->CaptureKeys(true);

    return true;
}


void GameOver() {
    //stop all movement
    game->SetDropAccel(0.0f);
    game->SetDropInterval(0.0f);

    float startx = game->GetGrid()->GetGridXStart();
    startx -= 50;
    float starty = game->GetGrid()->GetGridYStart();
    starty += game->GetGrid()->GetGridBlockSize() * (GRID_HEIGHT / 2) - (100/2);

    glBindTexture(GL_TEXTURE_2D, game->GetTextures()[FONT_TEXTURE]);
    glListBase(game->GetTextureBase()-32);

    game->CaptureKeys(false);

    bool loop = true;
    do {
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UpdateDisplay();

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
        glColor4f(0.3f, 0.3f, 0.3f, 0.3f);

        glBegin(GL_QUADS);
            glVertex2f(startx, starty);
            glVertex2f(startx + 250, starty);
            glVertex2f(startx + 250, starty + 100);
            glVertex2f(startx, starty + 100);
        glEnd();

        //save where we are
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glBlendFunc(GL_ONE, GL_DST_ALPHA);

        glTranslated(startx + ((250.0f / 2.0f) - float((9.0f * 16.0f) / 2)), starty + ((100.0f / 2.0f) - (16.0f / 2.0f)), 0);
        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
        glCallLists(9, GL_UNSIGNED_BYTE, "GAME OVER");

        glPopMatrix();

        glDisable(GL_BLEND);

        glfwSwapBuffers();
        glfwSleep(0.003);

        loop = !glfwGetKey(GLFW_KEY_F2);
        if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
            //show the menu
            menu = new MenuControl();
            menu->EscDown = true;
            MenuLoop();
            loop = false;

            delete menu;
            menu = NULL;
        }
    } while(loop);

    game->StartNewGame();
}

void DrawBackground() {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, game->GetTextures()[MAIN_BG_TEXTURE]);
    glBegin(GL_QUADS);
        glTexCoord2i(0,1);
        glVertex2i(0,0);

        glTexCoord2i(1,1);
        glVertex2i(640,0);

        glTexCoord2i(1,0);
        glVertex2i(640,480);

        glTexCoord2i(0,0);
        glVertex2i(0,480);

    glEnd();
    glPopMatrix();
}

void UpdateDisplay() {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawBackground();
    DrawGrid(game->GetGrid(), game->GetTextures());
    DrawTetromino(game->GetGrid(), game->GetTetromino(), game->GetTextures());
    DrawPreview();
    DrawText();
}

//main game loop
//everything gets triggered from here
void GameLoop() {
    double interval = 0;
    double oldtime = 0;
    double newtime = 0;
    double lastDropTime = 0;
    int frames = 0;
    game->SetFPS(0);
    double sleeptime = 0.0;

    game->SetGameState(true);
    sound->Play(SOUND_MUSIC);

    glfwSleep(1);
    game->GetNextTetromino()->Create();
    do {
        if (game->GetPaused()) {
            newtime = -1;
            //printf("newtime: %f lastdrop: %f\n", newtime, lastDropTime);
        }
        else
            newtime = glfwGetTime();

        if ((newtime - oldtime) > 1.0 || frames == 0) {
            game->SetFPS((double)frames / (newtime - oldtime));
            frames = 0;
            oldtime = newtime;
        }
        frames++;

        if (game->GetFPS() > 80) {
            sleeptime += 0.00001;
            //printf("sleep: %f.4\n", sleeptime);
        }
        if (game->GetFPS() > 30 && game->GetFPS() < 60 ) {
            sleeptime -= 0.00001;
            //printf("sleep: %f.4\n", sleeptime);
        }

        glViewport(0, 0, 640, 480);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        interval = game->GetDropInterval();
        if (game->GetDropAccel() > 0) {
            interval *= game->GetDropAccel();
        }

        if (newtime - lastDropTime >= interval) {
            MoveDown(game->GetGrid(), game->GetTetromino());
            lastDropTime = newtime;
        }

        if (game->GetTetromino()->IsLocked()) {
            //clear the current tetromino and set the next to be the current
            game->GetTetromino()->Reset();
            game->SetTetromino(game->GetNextTetromino());

            //create a new "next" to draw the preview from
            game->GetNextTetromino()->Reset();
            game->GetNextTetromino()->Create();

            //reset drop accel so that new tetromino goes at normal speed
            //and not the speed the previous was going as it was locked.
            //would be better if i could simulate a GLFW_RELEASE
            game->SetDropAccel(0.0f);

            //game over check
            if (game->GetGrid()->CheckCollision(game->GetTetromino())) {
                GameOver();
            }
        }

        UpdateDisplay();
        glfwSwapBuffers();
        glfwSleep(sleeptime);

        game->SetGameState(/* !glfwGetKey(GLFW_KEY_ESC) && */
            glfwGetWindowParam(GLFW_OPENED) && game->IsGameRunning());
    } while (game->IsGameRunning());

    glDeleteLists(game->GetTextureBase() ,256);
}

void GLFWCALL KeyHandle(int key, int action) {
    game->GameKeyHandle(key, action);
}

void GameControl::GameKeyHandle(int key, int action) {
    //if they no longer want the piece to drop faster... stop it
    if (action == GLFW_RELEASE && key == GLFW_KEY_DOWN) {
        //set the accel to 0 and make the interval drop
        //right for the current level.
        game->SetDropAccel(0.0f);
        game->SetDropInterval(DROP_INTERVAL_DEFAULT - (game->GetLevel() * DROP_INTERVAL_INCREMENT));
    }

    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESC:

                menu = new MenuControl();
                menu->EscDown = true;

                MenuLoop();
                delete menu;

                break;
            case GLFW_KEY_UP:
                {
                    Tetromino *temp = new Tetromino();
                    memcpy(temp, game->GetTetromino(), sizeof(Tetromino));

                    int pos = temp->GetPosition();
                    pos++;
                    if (pos > temp->GetMaxPositions()) {
                        pos = 1;
                    }
                    temp->SetShape(temp->GetType(), pos);
                    if (!game->GetGrid()->CheckCollision(temp)) {
                        game->GetTetromino()->SetShape(game->GetTetromino()->GetType(), pos);
                        sound->Play(SOUND_ROTATE);
                    }
                }
                break;
            case GLFW_KEY_LEFT:
                {
                    LOCATION_T location = game->GetTetromino()->GetLocation();
                    Tetromino *temp = new Tetromino();
                    memcpy(temp, game->GetTetromino(), sizeof(Tetromino));

                    temp->SetGridLocation(location.GridX - 1, location.GridY);
                    if (!game->GetGrid()->CheckCollision(temp)) {
                        game->GetTetromino()->SetGridLocation(location.GridX - 1, location.GridY);
                        sound->Play(SOUND_MOVE);
                    }
                    delete temp;

                }
                break;
            case GLFW_KEY_RIGHT:
                {
                    LOCATION_T location = game->GetTetromino()->GetLocation();
                    Tetromino *temp = new Tetromino();
                    memcpy(temp, game->GetTetromino(), sizeof(Tetromino));

                    temp->SetGridLocation(location.GridX + 1, location.GridY);
                    if (!game->GetGrid()->CheckCollision(temp)) {
                        game->GetTetromino()->SetGridLocation(location.GridX + 1, location.GridY);
                        sound->Play(SOUND_MOVE);
                    }

                    delete temp;
                }
                break;
            case GLFW_KEY_DOWN:
                    //have to set the drop interval to the default
                    //otherwise drop calculation in main loop is too fast
                    game->SetDropInterval(1.0f);
                    game->SetDropAccel(0.02f);
                    sound->Play(SOUND_ACCEL);
                break;
            case GLFW_KEY_F2:
                game->StartNewGame();
                /*
                game->GetTetromino()->Reset();
                game->GetTetromino()->Create();
                game->CaptureKeys(true);
                game->GetGrid()->Reset();
                game->SetLevel(0);
                game->SetDropInterval(DROP_INTERVAL_DEFAULT - (game->GetLevel() * DROP_INTERVAL_INCREMENT));*/
                break;
            default:
                break;
        }
    }

    fflush(stdout);
}
