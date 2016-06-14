#include "TitleMenu.h"
#include "Keyboard.h"
#include "Arena.h"
#include <cstdio>

void do_nothing(){}

void begin_game()
{
    printf("Starting new game\n");
    change_state(new Arena);
}

/***************************************************************************//**
The default constructor sets the background as the main_menu picture found in
./assets/images/main_menu.png, loads the popping sound effect found in
./assets/samples/pop.wav, and loads the menu with buttons.
*******************************************************************************/
TitleMenu::TitleMenu()
{
    background = find_bitmap("main_menu");
    auto font = find_font("times48");

    // Create the menu
    Button *new_game = new Button(NULL, NULL, "New Game", find_font("times", 32),  BLUE, RED, WIDTH / 2 - 125, HEIGHT / 2 - 64, begin_game);
    Button *load_game = new Button(NULL, NULL, "Load Game", find_font("times", 32),BLUE, RED, WIDTH / 2 + 125, HEIGHT / 2 - 64, do_nothing);
    Button *options = new Button(NULL, NULL, "Options", find_font("times", 32),    BLUE, RED, WIDTH / 2, HEIGHT / 2, do_nothing);
    Button *controls = new Button(NULL, NULL, "Controls", find_font("times", 32),  BLUE, RED, WIDTH / 2, HEIGHT / 2 + 64, do_nothing);

    main_menu.addButton("new_game", new_game, "", "options", "", "load_game");
    main_menu.addButton("load_game", load_game, "", "options", "new_game", "");
    main_menu.addButton("options", options, "", "controls", "new_game", "load_game");
    main_menu.addButton("controls", controls, "options", "", "", "");
    pop = find_sample("pop");
}

/***************************************************************************//**
There is no dynamically allocated data ::TitleMenu owns, so the destructor does
nothing.
*******************************************************************************/
TitleMenu::~TitleMenu()
{
}

/***************************************************************************//**
Calls Menu::handleKey on the menu, and reads the keyboard for other 
miscellaneous key presses. Pushing the space bar plays a sound effect. Pushing
the escape key ends the game.
*******************************************************************************/
void TitleMenu::handleKey()
{
    if (check_key(ESC))
    {
        end_game();
    }

    if (check_key(SPACE_UP)){
        al_play_sample(pop.get(), 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    main_menu.handleKey();
}

/***************************************************************************//**
Calls Menu::update
*******************************************************************************/
void TitleMenu::update()
{
    main_menu.update();
}

/***************************************************************************//**
Draws the menu and the background image.
*******************************************************************************/
void TitleMenu::render() const
{
    al_draw_bitmap(background.get(), 0, 0, 0);

    main_menu.render();
}
