#ifndef MENU_CONTROL_H
#define MENU_CONTROL_H

#include <gl/glfw.h>

//Menu Types
#define MENU_NEW_GAME 0
#define MENU_SFX_TOGGLE 1
#define MENU_MUSIC_TOGGLE 2
#define MENU_EXIT 3

#define MENU_WIDTH 250
#define MENU_HEIGHT 100
#define NUM_MENU_ITEMS 4

class MenuControl {
    public:
        MenuControl();

        void DoMenuItem(int Type);
        void AdjustSelectedItem(int Change);
        int GetSelectedIndex();
        bool EscDown;
    private:
        int m_selectedIndex;
};

void MenuLoop();
void DrawMenu();

#endif
