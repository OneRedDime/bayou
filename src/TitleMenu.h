#pragma once
#include "State.h"
#include "Menu.h"
#include "Assets.h"

/***************************************************************************//**
TitleMenu serves as an example of how to create a new State class and use it
appropriately. It shows how to properly create a ::Menu and how to load it with
::Button objects, handle key presses and read the mouse, and shows how to use
::State to draw things to the screen.
*******************************************************************************/
class TitleMenu : public State
{
    public:
        TitleMenu();
        ~TitleMenu();

        void handleKey();
        void update();
        void render() const;

    private:
        Menu main_menu;
        shared_bitmap background;
        shared_sample pop;
};