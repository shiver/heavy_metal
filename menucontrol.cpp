#include "menucontrol.h"
#include "gamecontrol.h"
#include "soundcontrol.h"

#include <gl/glfw.h>
#include <stdio.h>

extern GameControl *game;
extern SoundControl *sound;
MenuControl *menu;

MenuControl::MenuControl() {
    m_selectedIndex = 0;
    EscDown = false;
}

void MenuControl::AdjustSelectedItem(int Change) {
    int result = m_selectedIndex + Change;

    if (result < 0)
        m_selectedIndex = NUM_MENU_ITEMS - 1;
    else if (result > NUM_MENU_ITEMS - 1)
        m_selectedIndex = 0;
    else
        m_selectedIndex = result;
}

int MenuControl::GetSelectedIndex() {
    return m_selectedIndex;
}

void DrawMenu() {
    int width, height;

    //scissor tests allow to render only in the viewport we need
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glfwGetWindowSize(&width, &height);

    glScissor((width / 2) - (MENU_WIDTH / 2), (height / 2) - (MENU_HEIGHT / 2), MENU_WIDTH, MENU_HEIGHT);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    UpdateDisplay();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float startx = float((width / 2) - (MENU_WIDTH / 2));
    float starty = float((height / 2) - (MENU_HEIGHT / 2));

    glPushMatrix();

    glDisable(GL_SCISSOR_TEST);
    //glDisable(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_DST_ALPHA);

    //glColor4f(1.0f, 1.0f, 1.0f, 0.1f);

    glBindTexture(GL_TEXTURE_2D, game->GetTextures()[FONT_TEXTURE]);
    glListBase(game->GetTextureBase()-32);

    glTranslated(startx + 20, starty + 15, 0);
    glPushMatrix();
    glCallLists(8, GL_UNSIGNED_BYTE, "NEW GAME");
    glPopMatrix();

    glTranslated(0, 20, 0);
    glPushMatrix();
    glCallLists(10, GL_UNSIGNED_BYTE, "SFX TOGGLE");
    glPopMatrix();

    glTranslated(0, 20, 0);
    glPushMatrix();
    glCallLists(12, GL_UNSIGNED_BYTE, "MUSIC TOGGLE");
    glPopMatrix();

    glTranslated(0, 20, 0);
    glPushMatrix();
    glCallLists(4, GL_UNSIGNED_BYTE, "EXIT");
    glPopMatrix();

    glPopMatrix();

    //draw the menu selected icon thingy
    glPushMatrix();
    glTranslated(startx + 10, starty + 20.5f + (20 * menu->GetSelectedIndex()), 0);
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(5, 0);
        glVertex2f(5, 5);
        glVertex2f(0, 5);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void MenuControl::DoMenuItem(int Type) {
    switch (Type) {
        case MENU_NEW_GAME:
            game->StartNewGame();
            break;
        case MENU_SFX_TOGGLE:
            sound->SetSFXState(!sound->IsSFXEnabled());
            break;
        case MENU_MUSIC_TOGGLE:
            if (sound->IsMusicEnabled())
                sound->Stop(SOUND_MUSIC);
            else
                sound->Play(SOUND_MUSIC);

            break;
        case MENU_EXIT:
            game->SetGameState(false);
            break;
        default:
            break;
    }
}

void MenuLoop() {
    game->SetPaused(true);

    game->CaptureKeys(false);

    bool menuRunning = true;
    bool UpPressed = false;
    bool DownPressed = false;

    do {

        DrawMenu();
        glfwSwapBuffers();

        if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
            if (!menu->EscDown) {
                menuRunning = false;
                //menu->EscDown = true;
            }
        }
        if (glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE) {
            menu->EscDown = false;
        }

        if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
            if (!UpPressed) {
                UpPressed = true;
                menu->AdjustSelectedItem(-1);
            }
        }
        if (glfwGetKey(GLFW_KEY_UP) == GLFW_RELEASE) {
            UpPressed = false;
        }

        if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
            if (!DownPressed) {
                DownPressed = true;
                menu->AdjustSelectedItem(1);
            }
        }

        if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_RELEASE) {
            DownPressed = false;
        }

        if (glfwGetKey(GLFW_KEY_ENTER)) {
            menu->DoMenuItem(menu->GetSelectedIndex());
            menuRunning = false;
        }

        glfwSleep(0.003);

    } while (menuRunning);

    game->CaptureKeys(true);
    game->SetPaused(false);
}
