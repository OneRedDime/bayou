#include "Vector3.h"
#include <math.h>

/***************************************************************************//**
The default constructor for Vector3 sets all values to 0
******************************************************************************/
Vector3::Vector3()
{
    x = 0; y = 0; z = 0;
}

/***************************************************************************//**
The overloaded constructor for Vector3.
******************************************************************************/
Vector3::Vector3(const float x, const float y, const float z)
{
    Vector3::x = x; Vector3::y = y; Vector3::z = z;
}

/***************************************************************************//**
Adds two Vector3's and returns the result
******************************************************************************/
Vector3 Vector3::operator + (const Vector3 &other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

/***************************************************************************//**
Subtracts a Vector3 from this and returns the result as a Vector3
******************************************************************************/
Vector3 Vector3::operator - (const Vector3 &other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

/***************************************************************************//**
Multiplies each component of the Vector3 by a scalar c
******************************************************************************/
Vector3 Vector3::operator * (const float c) const
{
    return Vector3(x * c, y * c, z * c);
}

/***************************************************************************//**
Computes the cross product of this and another Vector3 and returns the result
******************************************************************************/
Vector3 Vector3::cross(const Vector3 other) const
{
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

/***************************************************************************//**
Normalizes this
******************************************************************************/
void Vector3::normalize()
{
    float m = mag();
    if (m != 0)
    {
        x /= m; y /= m; z /= m;
    }
    else
    {
        x = 0; y = 0; z = 0;
    }
}

/***************************************************************************//**
Calculates the magnitude of this
******************************************************************************/
float Vector3::mag() const
{
    return sqrt(x * x + y * y + z * z);
}

/***************************************************************************//**
Computes the dot product of this and another Vector3
******************************************************************************/
float Vector3::dot(const Vector3 other) const
{
    return x * other.x + y*other.y + z*other.z;
}