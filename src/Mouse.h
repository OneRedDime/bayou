/***************************************************************************//**
@defgroup mouse_group Mouse
@ingroup input_group
Using the functions defined in Mouse, one can update and read the state of the
mouse buttons and location. Include the file Mouse.h to use the functions
documented here.
*******************************************************************************/
#pragma once
#include <allegro5/allegro.h>

/***************************************************************************//**
@ingroup mouse_group
Very similar to ::Keys; contains enumerations for mouse buttons. The left and 
right mouse buttons are supported by default.\n
Adding support for new mouse buttons has not been tested. If you try though, you
should first add the proper enumerations here, update ::NUM_MOUSE_BUTTONS if
your new values are greater than RIGHT_CLICK_UP, and then look at the function
::update_mouse.
*******************************************************************************/
enum Clicks{
    LEFT_CLICK, LEFT_CLICK_DOWN, LEFT_CLICK_UP,
    RIGHT_CLICK, RIGHT_CLICK_DOWN, RIGHT_CLICK_UP
};

/***************************************************************************//**
@ingroup mouse_group
Indicates the number of enumerations in ::Clicks. Set to RIGHT_CLICK_UP + 1 by
default.\n
*******************************************************************************/
const int NUM_MOUSE_BUTTONS = RIGHT_CLICK_UP + 1;

/*******************************************************************************
@fn void init_mouse()
Should be called once before the game loop begins.
@fn void update_mouse()
Reads in the state of the physical mouse. Should be called once per game loop.
@fn bool check_mouse_button(Clicks click)
Works very similar to ::check_key. Send in a ::Clicks enumeration, and this
function will tell you if that click is being "pushed".\n
ie. check_mouse_button(LEFT_CLICK_UP) will return true if the user has just
released the left click.
@fn int get_mouse_x()
Returns the x component of the mouse's location on the screen. (0, 0) is the
upper left hand corner.
@fn int get_mouse_y()
Returns the y component of the mouse's location on the screen. (0, 0) is the
upper left hand corner.
@fn int get_num_mouse_buttons_pressed()
Returns the total number of activated mouse buttons, whether they're held down,
just pushed down, or are being released.
*******************************************************************************/
void init_mouse();
void update_mouse();
bool check_mouse_button(Clicks click);
int get_mouse_x();
int get_mouse_y();
int get_num_mouse_buttons_pressed();