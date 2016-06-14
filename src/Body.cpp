#include "Body.h"
#include <algorithm>
#include <cstdio>
using std::min;
using std::max;

/***********************************************************************//**
* The default constructor for Body does nothing
***************************************************************************/
Body::Body()
{
}

Body::Body(
    Vector3 pos, Vector3 vel, Vector3 accel,
    Vector3 dims,
    float m,
    float r, 
    float sf, float df,
    float oif, Vector3 dif,
    bool c,
    bool s,
    bool t
)
{
    setPos(pos);
    setVel(vel);
    setAccel(accel);
    setDims(dims);
    setMass(m);
    setRest(r);
    setSFric(sf);
    setDFric(df);
    setOmniImpelForce(oif);
    setDirImpelForce(dif);
    setCollidable(c);
    setStatic(s);
    setTangible(t);
}

/***********************************************************************//**
* The destructor does nothing; Body has no dynamically allocated data.
***************************************************************************/
Body::~Body()
{
}

/***********************************************************************//**
* Each field of dims must be non-negative
***************************************************************************/
void Body::setDims(Vector3 dims)
{
    if (dims.x >= 0 && dims.y >= 0 && dims.z >= 0)
    {
        dimensions = dims;
    }
    else
    {
        printf("Invalid dimensions: %f %f %f\n", dims.x, dims.y, dims.z);
    }
}

/***********************************************************************//**
* Mass cannot be less than or equal to 0. Mass of this will default to 1
* if sent a value of 0 or below.
***************************************************************************/
void Body::setMass(float m)
{
    if (m <= 0)
        mass = 1;
    else
        mass = m;
}

/***********************************************************************//**
* Restituion must be a value between 0 and 1 inclusive. It will be set to 0
* if sent a value outside this range.
***************************************************************************/
void Body::setRest(float r)
{
    restitution = r * (r >= 0 && r <= 1);
}

/***********************************************************************//**
* The coeffecient of static friction must be greater than or equal to 0.
* The value will default to 0 if passed a negative value.
***************************************************************************/
void Body::setSFric(float f)
{
    static_friction = f * (f >= 0);
}

/***********************************************************************//**
* Same rules as static friction
***************************************************************************/
void Body::setDFric(float d)
{
    dynamic_friction = d * (d >= 0);
}

/***********************************************************************//**
Calculates new acceleration based on this mass and the compelling_forces.
Updates velocity based on acceleration, position based on velocity, then
resets compelling_forces to 0.
***************************************************************************/
void Body::update()
{
    Vector3 da = compelling_forces * (1 / getMass()) * !is_static;
    acceleration = acceleration + da;
    velocity = getVel() + getAccel();
    position = position + velocity;

    acceleration = acceleration - da;
    compelling_forces = Vector3(0, 0, 0);
}

/***********************************************************************//**
* Applies compelling forces to this body's acceleration, then updates
* velocity based on acceleration, updates position based on velocity, and 
* then zeroes out the compelling force Vector3. 
***************************************************************************/
bool Body::checkCollision(const Body other) const
{
    Vector3 pos1 = getPos();
    Vector3 dims1 = getDims();

    Vector3 pos2 = other.getPos();
    Vector3 dims2 = other.getDims();

    if (other.isCollidable()
        && pos1.x + dims1.x / 2                    > pos2.x - dims2.x / 2
        && pos2.x + dims2.x / 2                    > pos1.x - dims1.x / 2
        && pos1.z + dims1.z / 2                    > pos2.z - dims2.z / 2
        && pos2.z + dims2.z / 2                    > pos1.z - dims1.z / 2
        && pos1.y + dims1.y / 2                    > pos2.y - dims2.y / 2 
        && pos2.y + dims2.y / 2                    > pos1.y - dims1.y / 2)
    {
        //don't have a print here. seriously don't.
        return true;
    }
    else
        return false;
}