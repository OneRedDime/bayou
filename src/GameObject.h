#pragma once

#include "Animation.h"
#include "Body.h"

enum Id { BOUNDRY, OBJECT, };

/***************************************************************************//**
GameObject represents the general object in the game. You don't have to program
your game using this GameObject, but it does have physics and works with
::Environment and has a ::Body. If you want to use Bayou's built in physics,
then each of your game's objects should derive from this.\n
There are several properties to a GameObject\n
- id Field of type ID, which is an enumeration indicating what type of object
  this is.
- body GameObjects physical properties. Read documentation for ::Body to find
out more about physical properties.
- active_animation Pointer to animation object currently playing for this object
- screen_x Offset indicating how far to the left the animation should be drawn.\n
  ie. 
- screen_y Same as screen_x, but to the up.
- alive Boolean indicating if the object is still active. The containing
::Environment will delete the object if it isn't alive.
*******************************************************************************/
class GameObject
{
    public:
        GameObject(
            Id id,
            Body body,
            Animation *animation,
            float s_x, float s_y
        );
        virtual ~GameObject();

        /* Getters */
        Id getId() const { return id; }
        const Animation *getActiveAnimation() const { return active_animation; }
        Body getBody() const { return body; }
        float getScreenX() const { return screen_x; }
        float getScreenY() const { return screen_y; }

        /* Body getters*/
        Vector3 getPos() const { return body.getPos(); }
        float getPosX() const { return body.getPos().x; }
        float getPosY() const { return body.getPos().y; }
        float getPosZ() const { return body.getPos().z; }
        Vector3 getVel() const { return body.getVel(); }
        float getVelX() const { return body.getVel().x; }
        float getVelY() const { return body.getVel().y; }
        float getVelZ() const { return body.getVel().z; }
        Vector3 getAccel() const { return body.getAccel(); }
        float getAccelX() const { return body.getAccelX(); }
        float getAccelY() const { return body.getAccelY(); }
        float getAccelZ() const { return body.getAccelZ(); }
        Vector3 getDims() const { return body.getDims(); }
        float getDimsX() const { return body.getDims().x; }
        float getDimsY() const { return body.getDims().y; }
        float getDimsZ() const { return body.getDims().z; }
        float getMass() const { return body.getMass(); }
        float getRest() const { return body.getRest(); }
        bool isAlive() const { return is_alive; }
        bool isCollidable() const { return body.isCollidable() && isAlive(); }
        bool isStatic() const { return body.isStatic(); }
        bool isTangible() const { return body.isTangible(); }

        /* Setters */
        void setId(const Id i) { id = i; }
        void setActiveAnimation(Animation *animation) { active_animation = animation; }
        void setBody(Body body) { GameObject::body = body; }
        void setScreenX(float x) { screen_x = x; }
        void setScreenY(float y) { screen_y = y; }
        void setAlive(bool a) { is_alive = a; }

        /* Body setters */
        void setPos(Vector3 p)            { body.setPos(p); }
        void setPosX(float x)            { body.setPosX(x); }
        void setPosY(float y)            { body.setPosY(y); }
        void setPosZ(float z)            { body.setPosZ(z); }
        void setVel(Vector3 v)            { body.setVel(v); }
        void setVelX(float x)            { body.setVelX(x); }
        void setVelY(float y)            { body.setVelY(y); }
        void setVelZ(float z)            { body.setVelZ(z); }
        void setAccel(Vector3 a)            { body.setAccel(a); }
        void setAccelX(float a)            { body.setAccelX(a); }
        void setAccelY(float a)            { body.setAccelY(a); }
        void setAccelZ(float a)            { body.setAccelZ(a); }
        void setDims(Vector3 d)            { body.setDims(d);}
        void setMass(float m)            { body.setMass(m);}
        void setRest(float r)            { body.setRest(r);}
        void setCollidable(bool c)        { body.setCollidable(c); }
        void setStatic(bool s)            { body.setStatic(s); }
        void setTangible(bool t)        { body.setTangible(t); }

        void applyForce(Vector3 force)    { body.applyForce(force); }

/***************************************************************************//**
@fn virtual void update() = 0
Should be called once per game cycle. This update update's the object's
currently playing animation and its ::Body.
@fn virtual void render(float scale = 1) const
Draws the object's currently playing animation. The optional parameter \p scale
will draw the animation at that scale in both x and y.
*******************************************************************************/
        virtual void update() = 0;
        virtual void render(float scale = 1) const;

/***************************************************************************//**
@fn bool checkCollision(const GameObject* otherObject) const
Returns true if the object's bodies are colliding.
@fn virtual void collided(const GameObject* object) = 0
Does not perform physical collision resolution. Use this for other things that
should happen when this objects collides with another.\n
ie. Have the character take damage if they touch fire.
@fn void collided(const Body body)
Perform game object logic as to what these objects should do when they collide.
*******************************************************************************/
        bool checkCollision(const GameObject* otherObject) const;
        virtual void collided(const GameObject* object) = 0;

    private:
        /* Data in init */
        Id id;
        Body body;
        Animation *active_animation;
        float screen_x, screen_y;
        bool is_alive;
};