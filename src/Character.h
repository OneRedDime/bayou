#pragma once
#include "GameObject.h"
class Character : public GameObject
{
    public:
        Character();
        ~Character();

        void update();
        void render(float scale = 1) const;
        void collided(const GameObject* object);
    private:
        Animation *animation;
};

