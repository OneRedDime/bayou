#include "Mouse.h"

void read_button(int keycode, bool &held, bool &down, bool &released);

ALLEGRO_MOUSE_STATE mouse_state;
/***************************************************************************//**
@var mouse
@ingroup mouse_group
Each value in ::Clicks corresponds to a location in this array. A given location
in this array is true if its "Click" has been activated. This array is
inaccessible to the rest of the program directly, and can only be read using
::check_mouse_button and written using ::update_mouse.
*******************************************************************************/
bool mouse[NUM_MOUSE_BUTTONS];
int num_pressed;

/***************************************************************************//**
@fn void init_mouse()
@ingroup mouse_group
Should be called once before the game loop begins. This initializes each index
in the ::mouse array to false.
*******************************************************************************/
void init_mouse()
{
    num_pressed = 0;
    for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
    {
        mouse[i] = false;
    }
}

/***************************************************************************//**
@fn void update_mouse()
@ingroup mouse_group
Reads in the state of the physical mouse. Should be called once per game loop.
*******************************************************************************/
void update_mouse()
{
    // Read mouse just before the update
    al_get_mouse_state(&mouse_state);
    num_pressed = 0;

    read_button(1, mouse[LEFT_CLICK], mouse[LEFT_CLICK_DOWN], mouse[LEFT_CLICK_UP]);
    read_button(2, mouse[RIGHT_CLICK], mouse[RIGHT_CLICK_DOWN], mouse[RIGHT_CLICK_UP]);
}

/***************************************************************************//**
@fn bool check_mouse_button(Clicks click)
@ingroup mouse_group
Works very similar to ::check_key. Send in a ::Clicks enumeration, and this
function will tell you if that mouse button is being "pushed". This information
is only accurate if ::update_mouse has been called once in this game cycle.\n
ie. check_mouse_button(LEFT_CLICK_UP) will return true if the user has just
released the left click. Both check_mouse_button(LEFT_CLICK_DOWN) and 
check_mouse_button(LEFT_CLICK) will return true if the user has just pushed
the left click.

@fn int get_mouse_x()
@ingroup mouse_group
Returns the x component of the mouse's location on the screen. (0, 0) is the
upper left hand corner.

@fn int get_mouse_y()
@ingroup mouse_group
Returns the y component of the mouse's location on the screen. (0, 0) is the
upper left hand corner.

@fn int get_num_mouse_buttons_pressed()
@ingroup mouse_group
Returns the total number of activated mouse buttons, whether they're held down,
just pushed down, or are being released. ::mouse[IDLE] will be true when this
function returns 0.
*******************************************************************************/
bool check_mouse_button(Clicks click)
{
    return mouse[click];
}
int get_mouse_x()
{
    return mouse_state.x;
}
int get_mouse_y()
{
    return mouse_state.y;
}
int get_num_mouse_buttons_pressed()
{
    return num_pressed;
}

/*******************************************************************************
Helper functions
*******************************************************************************/
void read_button(int keycode, bool &held, bool &down, bool &released)
{
    if (mouse_state.buttons & keycode && !held) // If button just pushed down
    {
        held = true;
        down = true;
        num_pressed++;
    }
    else if (!(mouse_state.buttons & keycode) && held) // If button was just released
    {
        held = false;
        released = true;
    }
    else if (mouse_state.buttons & keycode) // Button continues to be held down
    {
        held = true;
        down = false;
        num_pressed++;
    }
    else if (!(mouse_state.buttons & keycode))
    {
        held = false;
        down = false;
        released = false;
    }
}