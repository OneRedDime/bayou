#pragma once
#include "Manager.h"

/***************************************************************************//**
Bayou games are developed by creating new ::State instances, and each one of
those should derive from the State class. This class just guarantees a similar
interface so the Manager can use all states the same way.There are 3 methods 
each state must have in common.  If you are confused about proper State
creation, check out the code for ::TitleMenu in TitleMenu.h and TitleMenu.cpp.
*******************************************************************************/
class State
{
    public:
/***************************************************************************//**
@fn virtual State()
State's default constructor does nothing.
@fn virtual ~State()
The destructor here does nothing. It is only to guarantee the sub-class
destructors get called.
@fn virtual void handleKey() = 0
This function should be responsible for reading keyboard/mouse input and
interpreting that input.
@fn virtual void update() = 0
This is where the backend of your state should happen. This is where your State
should call Animation::update, or GameObject::update, or
Environment::updateObjects...
@fn virtual void render() const = 0
This class will only be responsible for drawing bitmaps to the screen.
*******************************************************************************/
        State(){};
        virtual ~State(){};
        virtual void handleKey() = 0;
        virtual void update() = 0;
        virtual void render() const = 0;
};