#include "Manager.h"
#include <cstdio>
#include <memory>
#include <stack>
using namespace std;

stack <unique_ptr<State> > states;

bool is_game_over()
{
    return states.empty();
}

void end_game()
{
    while (!is_game_over())
        states.pop();
}

/*******************************************************************************
FUNCTION change_state
********************************************************************************
DESCRIPTION: Changes state to new_state. Assumes new_state is initialized
*******************************************************************************/
void change_state(State *new_state)
{
    if (new_state)
    {
        pop_state();
        push_state(new_state);
    }
    else
        printf("Cannot change to NULL state!\n");
}

/*******************************************************************************
FUNCTION push_state
********************************************************************************
DESCRIPTION: pushes new_state onto the stack. This preserves all states below.
*******************************************************************************/
void push_state(State *new_state)
{
    if (new_state)
    {
        states.push(std::unique_ptr<State>(new_state));
    }
    else
        printf("Cannot push a NULL state!\n");
}

/*******************************************************************************
FUNCTION pop_state
********************************************************************************
DESCRIPTION: Deletes the top state.
*******************************************************************************/
void pop_state()
{
    if (states.size() > 0) //if there is an existing state
    {
        states.pop();
    }
    else
        printf("Cannot pop from an empty stack!\n");
}

void handle_key()
{
    if (!is_game_over())
        states.top()->handleKey();
}
void update_game()
{
    if (!is_game_over())
        states.top()->update();
    else
        end_game();
}
void render_game()
{
    if (!is_game_over())
        states.top()->render();
}
