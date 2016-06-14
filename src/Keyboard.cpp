#include "Keyboard.h"
#include <allegro5/allegro.h>
#include <functional>
using namespace std;
using namespace std::placeholders;

void read_key(ALLEGRO_KEYBOARD_STATE *state, int keycode, bool &held,
    bool &down, bool &released);

ALLEGRO_KEYBOARD_STATE keyboard_state;
/***************************************************************************//**
@var keyboard
@ingroup keyboard_group
Array of bools where each location corresponds to a value in ::Keys. The indices
here will be true if, after calling ::update_keyboard, the keys mentioned in
::Keys were activated. This variable is inaccessible to the rest of the program,
and can only be read using ::check_key, and written using ::update_keyboard.
*******************************************************************************/
bool keyboard[NUM_KEYS];
int num_keys_pressed;

/***************************************************************************//**
@fn void init_keyboard()
@ingroup keyboard_group
This function should be called before the game loop begins. Initializes each
index in ::keyboard to false.
*******************************************************************************/
void init_keyboard()
{
    num_keys_pressed = 0;
    for (int i = 0; i < NUM_KEYS; i++)
    {
        keyboard[i] = false;
    }
}

/***************************************************************************//**
@fn void update_keyboard()
@ingroup keyboard_group
This function updates the state of the keyboard, and should be called once per
game cycle.\n
You will also modify this function to add new keys to your game.
The recommended naming convention when adding new keys is to use the key's name
to correspond when that key is being continuously held down, *_DOWN to indicate
when the key has just been pushed, and *_UP to indicate when the key has just
been released.
For example: if I wanted to add the U key to my game I would add the following
line to the function:
\verbatim
// rk is an std::function bound within update_keyboard
// U is for when the u key is continuously held down, U_DOWN is when u has just
// been pressed, and U_UP is when u has just been released.
rk(ALLEGRO_KEY_U, keyboard[U], keyboard[U_DOWN], keyboard[U_UP]);
\endverbatim
This function will require knowledge of the proper Allegro keycode for the new
key.
*******************************************************************************/
void update_keyboard()
{
    auto rk = bind(read_key, &keyboard_state, _1, _2, _3, _4);
    al_get_keyboard_state(&keyboard_state);
    num_keys_pressed = 0;

    rk(ALLEGRO_KEY_A, keyboard[A], keyboard[A_DOWN], keyboard[A_UP]);
    rk(ALLEGRO_KEY_D, keyboard[D], keyboard[D_DOWN], keyboard[D_UP]);
    rk(ALLEGRO_KEY_P, keyboard[P], keyboard[P_DOWN], keyboard[P_UP]);
    rk(ALLEGRO_KEY_S, keyboard[S], keyboard[S_DOWN], keyboard[S_UP]);
    rk(ALLEGRO_KEY_W, keyboard[W], keyboard[W_DOWN], keyboard[W_UP]);
    rk(ALLEGRO_KEY_1, keyboard[ONE], keyboard[ONE_DOWN], keyboard[ONE_UP]);
    rk(ALLEGRO_KEY_2, keyboard[TWO], keyboard[TWO_DOWN], keyboard[TWO_UP]);
    rk(ALLEGRO_KEY_3, keyboard[THREE], keyboard[THREE_DOWN],
        keyboard[THREE_UP]);
    rk(ALLEGRO_KEY_4, keyboard[FOUR], keyboard[FOUR_DOWN], keyboard[FOUR_UP]);
    rk(ALLEGRO_KEY_ESCAPE, keyboard[ESC], keyboard[ESC_DOWN], keyboard[ESC_UP]);
    rk(ALLEGRO_KEY_ENTER, keyboard[ENTER], keyboard[ENTER_DOWN],
        keyboard[ENTER_UP]);
    rk(ALLEGRO_KEY_SPACE, keyboard[SPACE], keyboard[SPACE_DOWN],
        keyboard[SPACE_UP]);
    rk(ALLEGRO_KEY_LEFT, keyboard[LEFT], keyboard[LEFT_DOWN],
        keyboard[LEFT_UP]);
    rk(ALLEGRO_KEY_RIGHT, keyboard[RIGHT], keyboard[RIGHT_DOWN], 
        keyboard[RIGHT_UP]);
    rk(ALLEGRO_KEY_UP, keyboard[UP], keyboard[UP_DOWN], keyboard[UP_UP]);
    rk(ALLEGRO_KEY_DOWN, keyboard[DOWN], keyboard[DOWN_DOWN],
        keyboard[DOWN_UP]);
    rk(ALLEGRO_KEY_LSHIFT, keyboard[L_SHIFT], keyboard[L_SHIFT_DOWN],
        keyboard[L_SHIFT_UP]);
    rk(ALLEGRO_KEY_RSHIFT, keyboard[R_SHIFT], keyboard[R_SHIFT_DOWN],
        keyboard[R_SHIFT_UP]);

    keyboard[IDLE] = num_keys_pressed == 0;
}

/***************************************************************************//**
@fn bool check_key(Keys key)
@ingroup keyboard_group
Checks the ::keyboard array to see if the key at the specified location has been
activated. This information is only accurate if ::update_keyboard has been
called in the game loop once.
*******************************************************************************/
bool check_key(Keys key)
{
    return keyboard[key];
}

/***************************************************************************//**
@fn int get_num_keys_pressed()
@ingroup keyboard_group
Returns the total number of keys being activated. IDLE is not included.
*******************************************************************************/
int get_num_keys_pressed()
{
    return num_keys_pressed;
}

void read_key(ALLEGRO_KEYBOARD_STATE *state, int keycode, bool &held, bool
    &down, bool &released)
{
    if (al_key_down(state, keycode) && !held)
    {
        held = true;
        down = true;
        num_keys_pressed++;
    }
    else if (!al_key_down(state, keycode) && held)
    {
        held = false;
        released = true;
    }
    else if (al_key_down(state, keycode))
    {
        held = true;
        down = false;
        num_keys_pressed++;
    }
    else if (!al_key_down(state, keycode))
    {
        held = false;
        down = false;
        released = false;
    }
}
