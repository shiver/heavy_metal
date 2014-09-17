#include <stdio.h>

#include <gl/glfw.h>

#include "gamecontrol.h"
#include "soundcontrol.h"
#include "main.h"

extern GameControl *game;
extern SoundControl *sound;

int main() {
    game = new GameControl();
    sound = new SoundControl();

    if (!SetupScene(game->GetTextureBase(), game->GetTextures())) {
        printf("Setup failed\n");
        glfwTerminate();
        return 0;
    }

    GameLoop();

    glfwTerminate();

    return 0;
}
