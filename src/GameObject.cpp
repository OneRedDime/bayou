#include "GameObject.h"

GameObject::GameObject(
    Id id,
    Body body,
    Animation *animation,
    float s_x, float s_y
    )
{
    setId(id);
    setBody(body);
    setActiveAnimation(animation);
    setScreenX(s_x);
    setScreenY(s_y);
    setAlive(true);
}

GameObject::~GameObject()
{
}

/*******************************************************************************
FUNCTION update
********************************************************************************
DESCRIPTION: Updates physics and animation.
*******************************************************************************/
void GameObject::update()
{
    body.update();
    if (active_animation)
    {
        active_animation->update();
    }
}

/*******************************************************************************
FUNCTION render
********************************************************************************
DESCRIPTION: Draws the current frame of active_animation based on the object's
location.
*******************************************************************************/
void GameObject::render(float scale) const
{
    if (active_animation)
    {
        active_animation->render(
            body.getPosX() - screen_x,
            body.getPosY() - body.getPosZ() - screen_y,
            scale,
            scale);
    }
}

/*******************************************************************************
FUNCTION checkCollision
********************************************************************************
DESCRIPTION: Returns true if two GameObject bodies are colliding.
INPUT ARGS: otherObject
OUTPUT ARGS: none
IN/OUT ARGS: none
RETURN: bool
*******************************************************************************/
bool GameObject::checkCollision(const GameObject *other) const
{
    return body.checkCollision(other->getBody());
}