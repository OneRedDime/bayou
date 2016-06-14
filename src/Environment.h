#pragma once
#include "GameObject.h"
#include <vector>

/***************************************************************************//**
Environment contains a collection of ::GameObject pointers. It also defines a
minimal set of rules the objects must play by; namely gravity. Any ::GameObject
derivative should belong to an Environment, as Environment has built in methods
to collide and render groups of objects.\n
It should also be noted that each Environment takes ownership of its
GameObjects. If you want to preserve a persistent ::GameObject (ie. the player),
then you should take care to \link Environment::remove remove\endlink that
object from the Environment before the Environment is destroyed.
*******************************************************************************/
class Environment
{
    public:
/***************************************************************************//**
@fn Environment()
Initializes a new ::Environment instance with a gravity of 0 and no objects.
@fn ~Environment()
Destroys this and deletes all ::GameObject pointers within.
*******************************************************************************/
        Environment();
        ~Environment();

/***************************************************************************//**
@fn std::vector<GameObject *> getObjects() const
Returns the Environments collection of object pointers as a vector.
@fn float getGravity() const
Returns this Environment's gravity. This is how much each object in the
Environment will accelerate downward each game cycle.
*******************************************************************************/
        std::vector<GameObject *> getObjects() const { return objects; }
        float getGravity() const { return accel_gravity; }

/***************************************************************************//**
@fn void setGravity(float g)
Set the value for how much each object will accelerate downward each game cycle.
*******************************************************************************/
        void setGravity(float g) { accel_gravity = g; }

/***************************************************************************//**
@fn void pushBack(GameObject *object)
Adds a new ::GameObject pointer to the Environment. This will not reassign the
object's current ::Environment.
@fn void remove(const GameObject *object)
Searches for the ::GameObject in question, then removes the pointer. This does
not delete the object.
*******************************************************************************/
        void pushBack(GameObject *object) { objects.push_back(object); }
        void remove(const GameObject *object);

/***************************************************************************//**
@fn void updateObjects()
Updates all GameObjects in the ::Environment. Should be called once per game
cycle. Environment will also quicksort all objects by their y-values. This is so
the objects will render from back to front.
@fn void detectCollisions()
Checks all objects against each other to determine which ones could be
colliding. Populate a list of GameObject pointer pairs for potentially colliding
objects.
@fn void resolveCollisions()
Loops through all pairs in the collision pairs and collides them.
@fn void clean()
Removes and deletes all ::GameObject pointers which are not alive anymore.
@fn void renderObjects() const
Calls GameObject::render on all objects in this ::Environment. Will not modify
any objects.
*******************************************************************************/
        void updateObjects();
        void detectCollisions();
        void resolveCollisions();
        void clean();
        void renderObjects() const;

    private:
/*******************************************************************************
@fn void sort()
Quicksorts all GameObjects in the Environment according to their y values.
Environment does this during each update. This is so objects will render from
back to front.
*******************************************************************************/
        void sort();
        void destroyObjects();

        std::vector<GameObject *> objects;
        std::vector<std::pair<GameObject *, GameObject *> > collision_pairs;
        float accel_gravity;
};