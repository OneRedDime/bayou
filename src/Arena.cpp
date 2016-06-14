#include "Arena.h"
#include "Barrier.h"
#include "Globals.h"
#include "Character.h"
#include "Keyboard.h"
#include "TitleMenu.h"

Arena::Arena()
{
    image = find_bitmap("main_menu");
    environment.setGravity(-0.82);
    path_timer = 0;

    Barrier *floor, *ceiling, *west_wall, *east_wall, *north_wall, *south_wall, *center_wall;

    Body wall(
        Vector3(WIDTH / 2, HEIGHT / 2, -HEIGHT / 2),
        Vector3(), Vector3(), Vector3(WIDTH, HEIGHT, HEIGHT), 
        INT_MAX, 
        0.2, 
        0.7, 0.6, 
        0, 
        Vector3(), 
        true, true, true);

    floor = new Barrier(wall);
    environment.pushBack(floor);

    wall.setPos(Vector3(WIDTH / 2, HEIGHT / 2, HEIGHT * 1.5));
    ceiling = new Barrier(wall);
    environment.pushBack(ceiling);

    wall.setPos(Vector3(-WIDTH / 2, HEIGHT / 2, HEIGHT / 2));
    west_wall = new Barrier(wall);
    environment.pushBack(west_wall);

    wall.setPos(Vector3(WIDTH*1.5, HEIGHT / 2, HEIGHT / 2));
    east_wall = new Barrier(wall);
    environment.pushBack(east_wall);

    wall.setPos(Vector3(WIDTH / 2, HEIGHT * 1.5 - 110, HEIGHT / 2));
    south_wall = new Barrier(wall);
    environment.pushBack(south_wall);

    wall.setPos(Vector3(WIDTH / 2, -HEIGHT / 2 + 170, HEIGHT / 2));
    north_wall = new Barrier(wall);
    environment.pushBack(north_wall);

    // Invisible barrier at center of stage
    wall.setDims(Vector3(100, 200, HEIGHT / 2));
    wall.setPos(Vector3(WIDTH / 2, south_wall->getPosY() - south_wall->getDimsY() / 2 - wall.getDimsY() / 2, HEIGHT / 4));
    center_wall = new Barrier(wall);
    environment.pushBack(center_wall);

    // AI Mesh starts at the corner of the west and north walls, extends the whole floor past the north and south walls
    // and has tiling of 32
    mesh = new Mesh(0, 0, WIDTH / 32, HEIGHT / 32, 32);
    mesh->insertObject(center_wall);
    mesh->insertObject(north_wall);
    mesh->insertObject(south_wall);

    hero = new Character;
    hero->setPos(Vector3(100, HEIGHT/2, hero->getDimsZ() / 2));
    environment.pushBack(hero);

    elf = new Character;
    elf->setPos(Vector3(WIDTH / 2 + 300, HEIGHT / 2, HEIGHT / 2));
    environment.pushBack(elf);
}


Arena::~Arena()
{
    // We don't need to delete hero; environment will do that

    delete mesh;
}

void Arena::handleKey()
{
    if (check_key(ENTER))
        change_state(new TitleMenu);
    if (check_key(W))
        hero->applyForce(Vector3(0, -hero->getMass(), 0));
    if (check_key(A))
        hero->applyForce(Vector3(-hero->getMass(), 0, 0));
    if (check_key(S))
        hero->applyForce(Vector3(0, hero->getMass(), 0));
    if (check_key(D))
        hero->applyForce(Vector3(hero->getMass(), 0, 0));
    if (check_key(SPACE_DOWN))
        hero->applyForce(Vector3(0, 0, 25 * hero->getMass()));
}

void Arena::update()
{
    // Calc path from other character to ours so she can follow us
    if (path_timer % 60 == 0)
    {
        path = mesh->calcPath(elf->getPos(), hero->getPos());
        path_timer = 0;
    }
    path_timer++;

    if (!path.empty())
    {
        if ((path.back() - elf->getPos()).mag() < 10)
        {
            path.pop_back();
        }

        if (!path.empty())
        {
            // Now figure out where to push the elf to go along that path
            if (path.back().x < elf->getPosX())
            {
                elf->applyForce(Vector3(-elf->getMass(), 0, 0));
            }
            else
            {
                elf->applyForce(Vector3(elf->getMass(), 0, 0));
            }

            if (path.back().y < elf->getPosY())
            {
                elf->applyForce(Vector3(0, -elf->getMass(), 0));
            }
            else
            {
                elf->applyForce(Vector3(0, elf->getMass(), 0));
            }
        }
    }

    environment.updateObjects();
    environment.detectCollisions();
    environment.resolveCollisions();
}

void Arena::render() const
{
    al_draw_bitmap(image.get(), 0, 0, 0);
    environment.renderObjects();
}