#include "Character.h"
#include "Keyboard.h"
#include "Globals.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Character::Character() :
    GameObject(
    OBJECT, 
    Body(
        Vector3(0, 0, 0), 
        Vector3(0, 0, 0), 
        Vector3(0, 0, 0), 
        Vector3(32, 10, 64), 
        50, 
        0.25, 
        0.6, 0.7, 
        0, Vector3(0, 0, 0), 
        true, 
        false, 
        true),
    NULL, 
    32, 32
    )
{
    animation = new Animation(find_bitmap("elf_walk"), 10, 9);
    setActiveAnimation(animation);
}

Character::~Character()
{
    delete animation;
}

void Character::update()
{
    GameObject::update();
}

void Character::render(float scale) const
{
    al_draw_filled_ellipse(getPos().x, getPos().y, getDims().x / 2 * 1.3, getDims().y / 2 * 1.2, GREY);
    GameObject::render(scale);
}

void Character::collided(const GameObject* object)
{
}
