#pragma once
#include "GameObject.h"
class Barrier : public GameObject
{
    public:
        Barrier(Body body);
        ~Barrier();

        void update();
        void collided(const GameObject *other);
};

