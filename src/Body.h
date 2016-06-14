#pragma once
#include "Vector3.h"

/***************************************************************************//**
Body represents the collection of physical properties of an object. 
Body's various properties will be detailed here. \n
- position - Vector3 with x, y, z coordinates. This Vector3 indicates the 
    location of the center of the body.
- velocity - Indicates position of body on next update.\n
    Pos = (0, 0, 1) and Vel = (5, 5, -5), next pos = (5, 5, -4)\n
- acceleration- Indicates future change in velocity\n
- dimensions - Vector 3 indicating hitbox of object. All objects in Bayou are
    cubes.\n
- restitution - "Bounciness". Ratio indicating how much energy will be 
    preserved upon collision. When two objects collide, the lower restituion
    "wins" \n
    ie. Body A has rest of 1 and B has rest of 0.5. Each collides head on at
    speed 50 and has identical mass. Each will leave with speed 25.\n
    If they both has restitutions of 1, then the collision would be
    perfectly elastic.\n
- static_friction - Coeffecient of static_friction. Read a book if you don't
    know what this is.\n
- dynamic_friction - See static_friction\n
- omni_impel_force - Extra force an object may exert on all who touch it. An
    object with a high oif will repel all who touch it from wence they game.
    Think of a hand grenade explosion. If this value is negative then the
    object will attract other objects after touching them.
- dir_impel_force - Force vector which will apply a force to all contacting
    objects in a direction. Think of how a wind tunnel works.\n
- collidable - If set to true (almost always) the object can be interacted 
    upon. False means this object will ignore all collisions.\n
- static - If true then the body is static. Static objects cannot be 
    influenced by outside forces. They can move, and even accelerate, but 
    no contacting objects will be able to modify them. Static objects can 
    influence non-static objects (you can't run through a brick wall, 
    right?)\n
- tangible - If true then the object is solid. False = ghost. Non-tangible 
    objects will still exert their impel forces over tangible objects. ie: 
    wind can push you even though you can walk through it.\n
- compelling_forces - This is a Vector3 which is recalculated each update 
    tick.  It represents the sum of all forces acting upon this. When this
    is updated, the forces will be divided by the objects mass to
    determine its new acceleration.
There are a lot of setters and getters for Body. Most work as you would expect,
so only the ones with special rules will be detailed here.
*******************************************************************************/
class Body
{
    public:
        Body();
/***************************************************************************//**
@param pos ::Vector3 indicating initial position
@param vel ::Vector3 indicating initial velocity
@param accel ::Vector3 indicating initial acceleration
@param accel ::Vector3 indicating length, width, and height. Each must be
  non-negative
@param m Mass cannot be less than or equal to 0
@param r Restitution, aka bounciness factor. Must be between 0 and 1
@param sf Static friction. Must be non-negative
@param df Dynamic friction. Must be non-negative
@param oif Omni-impel force. Object will exert this force upon all other
  contacting objects such that they fly backwards.
@param dif Directional-impel force. Any objects contacting this will be
  thrown in the direction indicated by this Vector3. Think of how a wind
  tunnel works. It doesn't matter how you enter it, you are blown in one
  direction.
@param c Collidable. Objects that are not collidable will not be checked
  for collision by their environment
@param s Static. Static objects cannot ever be influenced by outside
  forces, though they can exert forces of their own.
@param t Tangible. True represents a solid object. False is like a ghost.
  Non-tangible objects can still exert their impel forces and be affected by
  other's impel forces. Non-tangible objects will still be stopped by static
  objects.
*******************************************************************************/
        Body(
            Vector3 pos, Vector3 vel, Vector3 accel,
            Vector3 dims,
            float m,
            float r,
            float sf, float df,
            float oif, Vector3 dif,
            bool c,
            bool s,
            bool t
        );

        ~Body();

        /* Getters */
        Vector3 getPos() const { return position; }
        float getPosX() const { return position.x; }
        float getPosY() const { return position.y; }
        float getPosZ() const { return position.z; }
        Vector3 getVel() const { return velocity; }
        float getVelX() const { return velocity.x; }
        float getVelY() const { return velocity.y; }
        float getVelZ() const { return velocity.z; }
        Vector3 getAccel() const { return acceleration; }
        float getAccelX() const { return acceleration.x; }
        float getAccelY() const { return acceleration.y; }
        float getAccelZ() const { return acceleration.z; }
        Vector3 getDims() const { return dimensions; }
        float getDimsX() const { return dimensions.x; }
        float getDimsY() const { return dimensions.y; }
        float getDimsZ() const { return dimensions.z; }
        float getMass() const { return mass; }
        float getRest() const { return restitution; }
        float getSFric() const { return static_friction; }
        float getDFric() const { return dynamic_friction; }
        float getOmniImpelForce() const { return omni_impel_force; }
        Vector3 getDirImpelForce() const { return dir_impel_force; }
        bool isCollidable() const { return is_collidable; }
        bool isStatic() const { return is_static; }
        bool isTangible() const { return is_tangible; }

        /* Setters */
        void setPos(Vector3 p) { position = p; }
        void setPosX(float x) { position.x = x; }
        void setPosY(float y) { position.y = y; }
        void setPosZ(float z) { position.z = z; }
        void setVel(Vector3 v) { velocity = v; }
        void setVelX(float x) { velocity.x = x; }
        void setVelY(float y) { velocity.y = y; }
        void setVelZ(float z) { velocity.z = z; }
        void setAccel(Vector3 a) { acceleration = a; }
        void setAccelX(float a) { acceleration.x = a; }
        void setAccelY(float a) { acceleration.y = a; }
        void setAccelZ(float a) { acceleration.z = a; }
        void setDims(Vector3 d);
        void setMass(float m);
        void setRest(float r);
        void setSFric(float f);
        void setDFric(float d);
        void setOmniImpelForce(float o) { omni_impel_force = o; }
        void setDirImpelForce(Vector3 i) { dir_impel_force = i; }
        void setCollidable(bool c){ is_collidable = c; }
        void setStatic(bool s) { is_static = s; }
        void setTangible(bool t){ is_tangible = t; }

        void applyForce(Vector3 force) { compelling_forces = (compelling_forces + force) * !is_static; }

        void update();

        bool checkCollision(const Body object) const;

    private:
        /* Base data */
        Vector3 position, velocity, acceleration;
        Vector3 dimensions;
        float mass;
        float restitution;
        float static_friction;
        float dynamic_friction;
        float omni_impel_force;
        Vector3 dir_impel_force;
        bool is_collidable;
        bool is_static;
        bool is_tangible;

        /* Derived data. Also SI units */
        Vector3 compelling_forces;
};
