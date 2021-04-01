#include "Environment.h"
#include <algorithm>
#include <cmath>
using std::min;
using std::max;

void collide_objects(Body &body1, Body &body2);
Vector3 calc_normal(const Body &body1, const Body &body2);
void position_correction(Body &body1, Body &body2);
float calc_least_penetration_depth(const Body &body1, const Body &body2);

bool compare(const GameObject *left, const GameObject *right)
{
    return left->getPosY() + left->getDimsY() / 2 < right->getPosY() + right->getDimsY() / 2;
}

/*******************************************************************************
Public methods
*******************************************************************************/

Environment::Environment()
{
    accel_gravity = 0;
}

/*******************************************************************************
FUNCTION Destructor
********************************************************************************
DESCRIPTION : Calls destroy_objects to clean the list
*******************************************************************************/
Environment::~Environment()
{
    destroyObjects();
}

/*******************************************************************************
FUNCTION remove
********************************************************************************
DESCRIPTION : Searches for the object pointer, and removes it from the list.
This only stops the list from including the object's pointer; it does not
delete the object.
*******************************************************************************/
void Environment::remove(const GameObject *object)
{
    for (auto it1 = objects.begin(); it1 != objects.end();)
    {
        if ((*it1) == object)
        {
            (*it1) = NULL;
            objects.erase(it1);
            return;
        }
        else
        {
            ++it1;
        }
    }
}

/*******************************************************************************
update_objects
********************************************************************************
DESCRIPTION : Sorts this, then calls update on all game objects in the list, or
removes them if they're not alive.
If an object removes itself as part of its update, the next object in
the list will get skipped. This is a minor bug, but has no currently known fix
*******************************************************************************/
void Environment::updateObjects()
{
    sort();

    int n = objects.size();
    for (int i = 0; i < n; i++)
    {
        Vector3 g_force(0, 0, objects[i]->getMass() * getGravity());
        objects[i]->applyForce(g_force);
        objects[i]->update();
        n = objects.size();
    }
}

/*******************************************************************************
FUNCTION detectCollisions
********************************************************************************
DESCRIPTION : Populates the collision_pairs list with pointers to colliding
objects.
*******************************************************************************/
void Environment::detectCollisions()
{
    collision_pairs.clear();

    for (auto it1 = objects.begin(); it1 != objects.end(); ++it1){
        if ((*it1)->isCollidable()) {
            for (auto it2 = it1; it2 != objects.end(); ++it2){
                if ((*it1)->isCollidable() && (*it2)->isCollidable() && (*it1) != (*it2)){
                    if ((*it1)->checkCollision((*it2))){
                        collision_pairs.push_back(std::pair<GameObject *, GameObject *>((*it1), (*it2)));
                    }
                }
            }
        }
    }
}

/*******************************************************************************
FUNCTION resolveCollisions
********************************************************************************
DESCRIPTION : Performs collision resolution for all objects in collision_pairs
*******************************************************************************/
void Environment::resolveCollisions()
{
    //check every object against every other object for a collision
    for (size_t i = 0; i < collision_pairs.size(); i++)
    {
        auto go1 = collision_pairs[i].first;
        auto go2 = collision_pairs[i].second;
        if (go1->checkCollision(go2))
        {
            Body b1 = go1->getBody();
            Body b2 = go2->getBody();
            collide_objects(b1, b2);
            go1->setBody(b1);
            go2->setBody(b2);
            go1->collided(go2);
            go2->collided(go1);
        }
    }
}

/*******************************************************************************
FUNCTION clean
********************************************************************************
DESCRIPTION : Removes and deletes all game objects where alive == false
*******************************************************************************/
void Environment::clean()
{
    for (auto it1 = objects.begin(); it1 != objects.end();)
    {
        if (!(*it1)->isAlive())
        {
            delete (*it1);
            (*it1) = NULL;
            it1 = objects.erase(it1);
        }
        else
        {
            ++it1;
        }
    }
}

/*******************************************************************************
FUNCTION render_objects
********************************************************************************
DESCRIPTION : Calls sort, then renders all of objects in the list.
*******************************************************************************/
void Environment::renderObjects() const
{
    for (auto it1 = objects.begin(); it1 != objects.end(); ++it1)
    {
        (*it1)->render();
    }
}

/*******************************************************************************
FUNCTION sort
********************************************************************************
DESCRIPTION : Sorts all objects by their y-value
*******************************************************************************/
void Environment::sort()
{
    std::sort(objects.begin(), objects.end(), compare);
}

/*******************************************************************************
Private methods
*******************************************************************************/

/*******************************************************************************
FUNCTION destroy_objects
********************************************************************************
DESCRIPTION : deletes all objects in the Environment.
*******************************************************************************/
void Environment::destroyObjects()
{
    for (auto it1 = objects.begin(); it1 != objects.end();)
    {
        delete (*it1);
        (*it1) = NULL;
        it1 = objects.erase(it1);
    }
}

void collide_objects(Body &body1, Body &body2)
{
    /* Create temporary copies. If the original objects are static
       then we don't want them modified */
    Body b1 = body1;
    Body b2 = body2;

    // Find normal direction
    Vector3 normal = calc_normal(b1, b2);

    // Calculate relative velocity
    Vector3 rv = b2.getVel() - b1.getVel();

    // Calculate relative velocity in terms of normal direction
    float velAlongNormal = rv.dot(normal);

    // Do not resolve if velocities are separating
    if (velAlongNormal > 0)
        return;

    // Calculate restitution
    float e = min(b1.getRest(), b2.getRest());

    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= 1 / b1.getMass() + 1 / b2.getMass();

    // Apply impulse to this object
    Vector3 impulse = normal * j;
    b1.setVel(b1.getVel() - impulse * (1 / b1.getMass()));
    b2.setVel(b2.getVel() + impulse * (1 / b2.getMass()));
    
    // Apply impel forces
    b1.applyForce(normal * -b2.getOmniImpelForce());
    b1.applyForce(b2.getDirImpelForce());
    b2.applyForce(normal * b1.getOmniImpelForce());
    b2.applyForce(b1.getDirImpelForce());

    // Apply positional correction
    position_correction(b1, b2);

    /* Apply friction */
    // Recalculate relative velocity
    rv = b2.getVel() - b1.getVel();

    // Solve for tangent vector
    Vector3 tangent = rv - (normal * rv.dot(normal));
    tangent.normalize();

    // Solve for frictional magnitude
    float jt = -rv.dot(tangent);
    jt /= (1 / b1.getMass() + 1 / b2.getMass());

    // Pythagorean solve for C (mu)
    float mu = sqrt(b1.getSFric() * b1.getSFric() + b2.getSFric() * b2.getSFric());

    // Clamp magniture of friction and create impulse vector
    Vector3 frictionImpulse;
    if (abs(jt) < j * mu)
    {
        frictionImpulse = tangent * jt;
    }
    else
    {
        float df = sqrt(b1.getDFric() * b1.getDFric() + b2.getDFric() * b2.getDFric());
        frictionImpulse = tangent * -j * df;
    }

    // Apply the impulse
    b1.setVel(b1.getVel() - frictionImpulse * (1 / b1.getMass()));
    b2.setVel(b2.getVel() + frictionImpulse * (1 / b2.getMass()));

    if (!body1.isStatic())
    {
        body1 = b1;
    }
    if (!body2.isStatic())
    {
        body2 = b2;
    }
}

Vector3 calc_normal(const Body &body1, const Body &body2)
{
    Vector3 n = body2.getPos() - body1.getPos();

    float a_x = body1.getDims().x / 2,
        b_x = body2.getDims().x / 2,
        a_y = body1.getDims().y / 2,
        b_y = body2.getDims().y / 2,
        a_z = body1.getDims().z / 2,
        b_z = body2.getDims().z / 2;

    // Calculate overlaps
    float x_overlap = a_x + b_x - abs(n.x);
    float y_overlap = a_y + b_y - abs(n.y);
    float z_overlap = a_z + b_z - abs(n.z);

    // Resolve overlap along the axis of least penetration
    if (x_overlap < y_overlap && x_overlap < z_overlap)
    {
        if (n.x < 0)
            return Vector3(-1, 0, 0);
        else
            return Vector3(1, 0, 0);
    }
    else if (y_overlap < x_overlap && y_overlap < z_overlap)
    {
        if (n.y < 0)
            return Vector3(0, -1, 0);
        else
            return Vector3(0, 1, 0);
    }
    else //if (z_overlap < x_overlap && z_overlap < y_overlap)
    {
        if (n.z < 0)
            return Vector3(0, 0, -1);
        else
            return Vector3(0, 0, 1);
    }
}

void position_correction(Body &body1, Body &body2)
{
    float percent = 1; // Usually 20% to 80%
    float slop = 0.01f; // Usually 0.01 to 0.1
    Vector3 n = calc_normal(body1, body2);
    float penetration = calc_least_penetration_depth(body1, body2);
    float inv_mass1 = 1 / body1.getMass();
    float inv_mass2 = 1 / body2.getMass();
    Vector3 correction = n * (max(penetration - slop, 0.0f) / (inv_mass1 + inv_mass2) * percent);

    body1.setPos(body1.getPos() - correction * inv_mass1);
    body2.setPos(body2.getPos() + correction * inv_mass2);
}

float calc_least_penetration_depth(const Body &body1, const Body &body2)
{
    Vector3 n = body2.getPos() - body1.getPos();

    float a_x = body1.getDimsX() / 2,
        b_x = body2.getDimsX() / 2,
        a_y = body1.getDimsY() / 2,
        b_y = body2.getDimsY() / 2,
        a_z = body1.getDimsZ() / 2,
        b_z = body2.getDimsZ() / 2;

    // Calculate overlaps
    float x_overlap = a_x + b_x - abs(n.x);
    float y_overlap = a_y + b_y - abs(n.y);
    float z_overlap = a_z + b_z - abs(n.z);

    return min(x_overlap, min(y_overlap, z_overlap));
}
