#pragma once
#include "GameObject.h"
#include <vector>
#include <utility>

struct MeshNode {
    Vector3 pos;
    std::vector<const GameObject *> objects;
    std::pair<int, int> coords; // Coordinates on mesh array
    std::pair<int, int> parent; // Parent's coordinates on network
    int f, g, h;

    MeshNode()
    {
        coords.first = -1;
        coords.second = -1;
        parent.first = -1;
        parent.second = -1;
        f = 0; g = 0; h = 0;
    }

    bool operator>(const MeshNode &other) const
    {
        return f > other.f;
    }
};


/***************************************************************************//**
The Mesh object is composed of a square grid of nodes, and is used for
pathfinding. Paths between points are calculated using the A* algorithm.

This class is in rough draft. Right now the path will always try to find a way
around any object. The final version should accept a function object to the
calculate any miscellanious interest in a node, which would be the value an AI
assigns to the node based on locations of other GameObjects.
*******************************************************************************/
class Mesh {
    public:
/***************************************************************************//**
@fn Mesh(int start_x, int start_y, int width, int thickness, int tiling)
Creates a new Mesh object. start_x and y indicate the coordinates of the upper
left hand corner. width and thickness are the dimensions of the mesh in number
of nodes. tiling is space between the nodes. Each mesh has a height hardcoded
at 32.
*******************************************************************************/
        Mesh(int start_x, int start_y, int width, int thickness, int tiling);
        ~Mesh();

/***************************************************************************//**
@fn void insertObject(const GameObject *new_object)
Inserts a new GameObject pointer onto the mesh. Each node this object overlaps
will gain a constant pointer to this object.
*******************************************************************************/
        void insertObject(const GameObject *new_object);

/***************************************************************************//**
@fn void clearObjects()
Removes all object pointers from the mesh.
*******************************************************************************/
        void clearObjects();

/***************************************************************************//**
@fn vector<Vector3> calcPath(Vector3 start, Vector3 target) const
Returns a vector of Vector3 structs representing the best path to take between
start and target. Will return an empty vector if start or target are out of
bounds.
*******************************************************************************/
        std::vector<Vector3> calcPath(Vector3 start, Vector3 target);

    private:

        std::vector<std::pair<int, int> > getNodesInObject(const GameObject *) const;
        std::pair<int, int> getIndicesFromPos(Vector3 pos) const;
        Vector3 getPosFromIndices(std::pair<int, int> coords) const;
        std::vector<std::pair<int, int> > getNeighbors(std::pair<int, int> center) const;
        bool inBounds(int x, int y) const;

        std::vector<std::vector<MeshNode> > network;
        int start_x, start_y;
        int height;
        int length, thickness, tiling;

        std::vector<const GameObject *> objects;
};