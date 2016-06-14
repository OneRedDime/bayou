#pragma once
#include "Button.h"
#include "HudElement.h"
#include <vector>
#include <map>
#include <memory>

/***************************************************************************//**
Menus allows users to traverse the different buttons using the arrow keys by 
maintaining a graph of Nodes. A node is a struct with 6 public members.
@param name Name of the node as it is stored in a ::Menu
@param button ::Button pointer
@param up Name of the node which the Menu will traverse to when the up arrow key
is pushed. If you don't want a button to the up, simply pass "".
@param down See up
@param left See up
@param right See up
*******************************************************************************/
struct Node
{
    std::string name;
    Button *button;
    std::string up, down, left, right;

    ~Node()
    {
        delete button;
    }
};

/***************************************************************************//**
Menu serves as a collection of ::Button and ::HudElement objects. While buttons
and HudElements can be used independently of a Menu object, you would be
responsible for updating each one explicitly. For example, Menu.update() will
update all Buttons and HudElements contained within.
*******************************************************************************/
class Menu
{
    public:
/***************************************************************************//**
@fn Menu()
Creates a new empty Menu.
@fn ~Menu()
Destroys Menu's ::Node graph.
*******************************************************************************/
        Menu();
        ~Menu();

/***************************************************************************//**
@fn void addButton(std::string name, Button *button, std::string up, std::string down, std::string left, std::string right);
Creates a new Node from the arguments and adds that to the node graph.
@param name Name of the new node. This will be used as an index for storage.
@param button Pointer to a ::Button. This ::Menu instance will be responsible
for deleting this ::Button.
@param up Name of Node which will be traveled to if the up arrow key is pushed
@param down See up.
@param left See up.
@param right See up.

@fn void addHudElement(HudElement *hud_element)
Adds a new ::HudElement to the Menu. This Menu instance will be responsible for
deleting this ::HudElement.
*******************************************************************************/
        void addButton(std::string name, Button *button, std::string up, std::string down, std::string left, std::string right);
        void addHudElement(HudElement *hud_element);

/***************************************************************************//**
@fn void handleKey()
::Menu maintains an active ::Button while the user traverses the different buttons
using the arrow keys. This button is always in the preactive state.\n
This function is responsible for checking the keyboard for input.
- Left will move the active node to the button mapped as its 'left', if it
  exists. See Menu::addButton and ::Node for details on the structure of
  Buttons.
- The other arrow keys do as you expecct.
- Pushing enter will activate the currently selected button
    - Left clicking the mouse will do the same if the mouse is hovering over a
      button. How ::Menu checks the mouse will be detailed in Menu::update.
@fn void update()
This function should be called once per update cycle.\n
Update's job is to call each ::HudElement update, and to check the position of
the mouse to see if it's over a button. If it is, then that button becomes the
new active button.
@fn void render() const
This function should be called once per update cycle.\n
Calls render on each ::HudElement and ::Button.
*******************************************************************************/
        void handleKey();
        void update();
        void render() const;

    private:
        std::vector<HudElement *> hud_elements;
        std::map <std::string, Node *> buttons;
        Node *active_button;
};