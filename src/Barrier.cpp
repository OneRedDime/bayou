#include "Barrier.h"


Barrier::Barrier(Body body) :
    GameObject(
        BOUNDRY,
        body,
        NULL,
        0, 0
    )
{
}


Barrier::~Barrier()
{
}

void Barrier::update()
{
    GameObject::update();
}

void Barrier::collided(const GameObject *other)
{
}