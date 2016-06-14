/***************************************************************************//**
A Vector3 is a mathematical vector with 3 components: x, y, and z.
This struct also has methods to add two Vector3's, compute the cross product,
the dot product, normalize, etc... \n
This struct is primarily used in Body, as most of the data for a physical
object is stored as Vector3's.
*******************************************************************************/
struct Vector3
{
/***************************************************************************//**
@var x
X component of Vector3. When used in a ::GameObject, this controls its position
left->right across the screen.
@var y
Y component of Vector3. When used in a ::GameObject, this controls its position
away->closer to the screen. Negative is away, positive is closer.
@var z
Z component of Vector3. When used in a ::GameObject, this controls the object's
elevation. Negative is downward, positive is upward.
*******************************************************************************/
    float x, y, z;

/***************************************************************************//**
The default constructor for Vector3 sets all values to 0
*******************************************************************************/
    Vector3();

/***************************************************************************//**
The overloaded constructor for Vector3.
*******************************************************************************/
    Vector3(const float x, const float y, const float z);

/***************************************************************************//**
Adds two Vector3's and returns the result
*******************************************************************************/
    Vector3 operator + (const Vector3 &other) const;

/***************************************************************************//**
Subtracts a Vector3 from this and returns the result as a Vector3
*******************************************************************************/
    Vector3 operator - (const Vector3 &other) const;

/***************************************************************************//**
Multiplies each component of the Vector3 by a scalar c
*******************************************************************************/
    Vector3 operator * (const float c) const;

/***************************************************************************//**
Computes the cross product of this and another Vector3 and returns the
result
*******************************************************************************/
    Vector3 cross(const Vector3 other) const;

/***************************************************************************//**
Returns true if this vector's 3 components equal the other vector's.
*******************************************************************************/
    bool operator==(const Vector3 &other)
    {
        return x = other.x && y == other.y && z == other.z;
    }

/***************************************************************************//**
Normalizes this
*******************************************************************************/
    void normalize();

/***************************************************************************//**
Calculates the magnitude of this
*******************************************************************************/
    float mag() const;

/***************************************************************************//**
Computes the dot product of this and another Vector3
*******************************************************************************/
     float dot(const Vector3 other) const;
};