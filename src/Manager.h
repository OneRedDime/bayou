/***************************************************************************//**
@defgroup manager_group Manager
Bayou games are developed by creating new ::State instances, but only one of
those states should be active at a time. The Manager module maintains a stack
of states, and defines a set of functions to interact with that stack. The state
at the top of the stack is referred to as the "active state".\n
Include the file Manager.h to use these functions.
*******************************************************************************/
#pragma once
#include "State.h"

class State;

/***************************************************************************//**
@fn bool is_game_over()
@ingroup manager_group
This function returns true when the game is over. This means there are no
::State objects in the stack. This will happen by calling ::end_game or by
having the last state \link ::pop_state pop\endlink itself.
@fn void end_game()
@ingroup manager_group
Ends the game by deleting all states pushed to the manager.
*******************************************************************************/
bool is_game_over();
void end_game();

/***************************************************************************//**
@fn void change_state(State *new_state)
@ingroup manager_group
Deletes the current state and replaces it with new_state. The Manager module
will take ownership of this state.
@fn void push_state(State *new_state)
@ingroup manager_group
Pushes new_state onto the stack, preserving the states below it. The Manager
will take ownership of this state.
@fn void pop_state()
@ingroup manager_group
Deletes the current ::State.
*******************************************************************************/
void change_state(State *new_state);
void push_state(State *new_state);
void pop_state();

/***************************************************************************//**
@fn void handle_key()
@ingroup manager_group
Calls State::handleKey on the active state.
@fn void update_game()
@ingroup manager_group
Calls State::update on the active state.
@fn void render_game()
@ingroup manager_group
Calls State::render on the active state.
*******************************************************************************/
void handle_key();
void update_game();
void render_game();