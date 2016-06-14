#pragma once
#include "State.h"
#include "Assets.h"
#include "Environment.h"
#include "Character.h"
#include "Mesh.h"
#include <vector>

class Arena : public State
{
    public:
        Arena();
        ~Arena();

        void handleKey();
        void update();
        void render() const;

    private:
        shared_bitmap image;
        Character *hero, *elf;
        Environment environment;
        Mesh *mesh;
        std::vector<Vector3> path;
        int path_timer;
};