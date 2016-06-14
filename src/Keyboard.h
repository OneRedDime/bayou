/***************************************************************************//**
@defgroup input_group Input
The Input module contains 2 primary sub-modules: Keyboard and Mouse. Each of
those sets of functions are defined in different .h files, but they are both
documented under Input since their purposes are almost identical.

@defgroup keyboard_group Keyboard
@ingroup input_group
The Keyboard module defines a suite of functions and enumerated data types for
reading in the state of the user's keyboard into memory, and then accessing
various information about that state. There are other functions to assist in
this process, but only the ones accessible throughout the rest of the program
are detailed here. Include the file Keyboard.h to use the functions detailed
here.
*******************************************************************************/
#pragma once

/***************************************************************************//**
@ingroup keyboard_group
Each enumeration in Keys corresponds to the state of the keys on the keyboard.
The primary use of this enumeration is to have its values passed to ::check_key.
Each key on your keyboard should have 3 corresponding values in Keys:
- Keys named *_DOWN are for checking when that key has just been pushed down.
- Keys named *_UP are for when that key has just been released.
- The other enumerations are for when that key is being held down.
- IDLE is for checking when no keys are held down.
- NUM_KEYS is the number of keys on the keyboard

Not all keys on the physical keyboard are usable by default. If you want to add
keys, you must add them to the ::Keys type. You're also going to have to modify
the function ::update_keyboard.
*******************************************************************************/
enum Keys {
    IDLE,
    A, A_DOWN, A_UP,
    D, D_DOWN, D_UP,
    P, P_DOWN, P_UP,
    S, S_DOWN, S_UP,
    W, W_DOWN, W_UP,
    ONE, ONE_DOWN, ONE_UP,
    TWO, TWO_DOWN, TWO_UP,
    THREE, THREE_DOWN, THREE_UP,
    FOUR, FOUR_DOWN, FOUR_UP,
    ESC, ESC_DOWN, ESC_UP,
    ENTER, ENTER_DOWN, ENTER_UP,
    SPACE, SPACE_DOWN, SPACE_UP,
    LEFT, LEFT_DOWN, LEFT_UP,
    RIGHT, RIGHT_DOWN, RIGHT_UP,
    UP, UP_DOWN, UP_UP,
    DOWN, DOWN_DOWN, DOWN_UP,
    L_SHIFT, L_SHIFT_DOWN, L_SHIFT_UP,
    R_SHIFT, R_SHIFT_DOWN, R_SHIFT_UP,

    NUM_KEYS
};

/*******************************************************************************
@fn void init_keyboard()
This function should be called before the game loop begins.
@fn void update_keyboard()
This reads in the state of the physical keyboard. This should be called once per
game loop. You will have to modify this function if you want to add new keys to
your game.
@fn bool check_key(Keys key)
Returns true if the Keys value you passed in is currently "pushed".\n
ie. If the user has just released the A key, the check_key(A_UP) is true.
@fn int get_num_keys_pressed()
Returns the total number of keys being activated. IDLE is not included.
*******************************************************************************/
void init_keyboard();
void update_keyboard();
bool check_key(Keys key);
int get_num_keys_pressed();