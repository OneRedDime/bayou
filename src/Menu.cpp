#include "Menu.h"
#include "Keyboard.h"
#include "Mouse.h"
using namespace std;

Menu::Menu()
{
}

Menu::~Menu()
{
    active_button = NULL;
    for (auto it = buttons.begin(); it != buttons.end();)
    {
        delete (*it).second;
        (*it).second = NULL;
        it = buttons.erase(it);
    }
}

void Menu::addButton(string name, Button *button, string up, string down, string left, string right)
{
    Node *new_button = new Node{name, button, up, down, left, right};
    active_button = new_button;
    buttons.insert(pair<string, Node *>(new_button->name, new_button));
}

void Menu::addHudElement(HudElement *hud_element)
{
    hud_elements.push_back(hud_element);
}

void Menu::handleKey()
{   
    bool activate = false;

    if (check_key(UP_UP)
        && buttons.find(active_button->up) != buttons.end())
    {
        active_button->button->setPreActive(false);
        active_button = buttons.find(active_button->up)->second;
    }
    else if (check_key(DOWN_UP)
        && buttons.find(active_button->down) != buttons.end())
    {
        active_button->button->setPreActive(false);
        active_button = buttons.find(active_button->down)->second;
    }
    else if (check_key(LEFT_UP)
        && buttons.find(active_button->left) != buttons.end())
    {
        active_button->button->setPreActive(false);
        active_button = buttons.find(active_button->left)->second;
    }
    else if (check_key(RIGHT_UP)
        && buttons.find(active_button->right) != buttons.end())
    {
        active_button->button->setPreActive(false);
        active_button = buttons.find(active_button->right)->second;
    }
    else if (check_key(ENTER_UP))
    {
        activate = true;
    }
    else if (check_mouse_button(LEFT_CLICK_UP)
        && active_button->button->getCursorHover())
    {
        activate = true;
    }

    if (activate)
        active_button->button->activate();
}

void Menu::update()
{
    for (auto it = hud_elements.begin(); it != hud_elements.end(); ++it)
    {
        (*it)->update();
    }
    for (auto it = buttons.begin(); it != buttons.end();)
    {
        if ((*it).second->button->getCursorHover())
        {
            active_button->button->setPreActive(false);
            active_button = buttons.find((*it).second->name)->second;
            break;
        }
        else
            ++it;
    }

    if(active_button)
        active_button->button->setPreActive(true);
}

void Menu::render() const
{
    // Render all hud elements
    for (auto it = hud_elements.begin(); it != hud_elements.end(); ++it)
    {
        (*it)->render();
    }

    // Then render buttons
    for (auto it = buttons.begin(); it != buttons.end(); ++it)
    {
        (*it).second->button->render();
    }
}
