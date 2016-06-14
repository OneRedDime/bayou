#pragma once
#include <memory>

/***************************************************************************//**
Bayou has 3 classes to assist with game menus/HUDS. HudElements is one of those.
The other two are ::Menu and ::Button.\n
HudElements represent the parts of a Menu or HUD that are read only; you can't
interact with them.\n
If you want to create a new HudElement, then you would create a new class that
derives from HudElement.\n
For example, lets say you want to create a health bar for your character. Your
class may look like
    \verbatim
    class HealthBar : public HudElement {
        HealthBar(const Player* player)
        ~HealthBar()
        void update();
        void render();
    }
    \endverbatim
*******************************************************************************/
class HudElement
{
    public:
        HudElement();
        virtual ~HudElement();

/***************************************************************************//**
@fn virtual void update() = 0
Any HudElement's update should be called once per game cycle. Use this function
to re-read data and set proper variables for HudElement::render to use.
@fn virtual void render() const = 0
Write your own render in your derived classes to properly display your relevant
information.
*******************************************************************************/
        virtual void update() = 0;
        virtual void render() const = 0;
};