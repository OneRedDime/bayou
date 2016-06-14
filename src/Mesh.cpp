#include "Mesh.h"
#include <cstddef>
#include <queue>
#include <algorithm>
#include <functional>
#include <unordered_map>
using std::unordered_map;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;

Mesh::Mesh(int start_x, int start_y, int length, int thickness, int tiling) {
    this->start_x = start_x;
    this->start_y = start_y;
    this->length = length;
    this->thickness = thickness;
    this->tiling = tiling;
    this->height = 32;

    for (int i = 0; i < length; i++)
    {
        network.push_back(vector<MeshNode>());
        for (int j = 0; j < thickness; j++)
        {
            MeshNode node;
            node.coords = pair<int, int>(i, j);
            node.pos = getPosFromIndices(node.coords);
            network[i].push_back(node);
        }
    }
}

Mesh::~Mesh()
{
}

/*******************************************************************************
Inserts a new GameObject pointer onto the mesh. Each node this object overlaps
will gain a constant pointer to this object.
*******************************************************************************/
void Mesh::insertObject(const GameObject *new_object)
{
    objects.push_back(new_object);
    auto coordinates = getNodesInObject(new_object);

    for (size_t i = 0; i < coordinates.size(); i++)
    {
        auto xy = coordinates[i];
        if (inBounds(xy.first, xy.second))
        {
            network[xy.first][xy.second].objects.push_back(new_object);
        }
    }
}

/*******************************************************************************
Finds all nodes contained within each GameObject in the objects vector and
clears their lists before clearing the objects vector
*******************************************************************************/
void Mesh::clearObjects()
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        auto nodes = getNodesInObject(objects[i]);
        for (size_t j = 0; j < nodes.size(); j++)
        {
            auto xy = nodes[j];
            network[xy.first][xy.second].objects.clear();
        }
    }
    objects.clear();
}

/*******************************************************************************
Uses A* to return a vector of Vector3's representing the path between start
and target. This will avoid all game objects by setting nodes f scores to
incredibly high values if they have objects on them. Needs to be updated to
allow passing of a custom function object to allow custom priorities for nodes.
You wouldn't want your ai to avoid healing potions, right?
*******************************************************************************/
vector<Vector3> Mesh::calcPath(Vector3 start, Vector3 target)
{
    vector<Vector3> path;
    pair<int, int> start_coords = getIndicesFromPos(start);
    pair<int, int> target_coords = getIndicesFromPos(target);

    // Return empty path if positions are out of bounds
    if (!inBounds(start_coords.first, start_coords.second)
        || !inBounds(target_coords.first, target_coords.second))
    {
        return path;
    }

    // Initialize the open list. These are nodes which may be examined.
    vector<MeshNode> open_list;

    // The path will be reconstructed from this once we find the goal
    // Any node addresses in this map are nodes we've examined.
    unordered_map <const MeshNode *, bool> closed_list;
    
    // Put starting node on open list
    MeshNode start_node = network[start_coords.first][start_coords.second];
    start_node.f = 0;
    start_node.g = 0;
    start_node.h = 0;
    open_list.push_back(network[start_coords.first][start_coords.second]);
    
    // While the open list is not empty
    while (!open_list.empty())
    {
        // Find the node with the least f on the open list, call it "q"
        MeshNode q = open_list.back();

        // Pop q off the open list
        open_list.pop_back();

        // Mark q on the closed list and save q to the network
        network[q.coords.first][q.coords.second] = q;
        closed_list[&network[q.coords.first][q.coords.second]] = true;

        // Generate q's neighbors and set their parents to q
        vector<pair<int, int> > neighbor_coords = getNeighbors(q.coords);

        // Test each neighbor
        for (size_t i = 0; i < neighbor_coords.size(); i++)
        {
            MeshNode successor;
            successor.coords = neighbor_coords[i];
            successor.parent = q.coords;
            successor.pos = network[successor.coords.first][successor.coords.second].pos;
            successor.objects = network[successor.coords.first][successor.coords.second].objects;

            // successor.g = q.g + distance between successor and q
            successor.g = q.g + (q.pos - successor.pos).mag();

            // successor.h = distance from goal to successor
            successor.h = (successor.pos - target).mag();

            // successor.f = successor.g + successor.h
            successor.f = successor.g + successor.h;

            // Set any nodes containing objects to really high f value
            if (!network[successor.coords.first][successor.coords.second].objects.empty())
            {
                successor.f += 10000000;
            }

            // if successor is the goal, stop the search
            if (successor.coords == target_coords)
            {
                // Add successor to the closed list
                network[successor.coords.first][successor.coords.second] = successor;
                closed_list[&network[successor.coords.first][successor.coords.second]] = true;

                // Emptying the open list to exit
                open_list.clear();
                break;
            }

            bool skip = false;

            // If a node with the same position as successor is in the OPEN list
            // which has a lower f than successor, skip this successor
            for (size_t j = 0; j < open_list.size(); j++)
            {
                if (open_list[j].coords == successor.coords
                    && open_list[j].f <= successor.f)
                {
                    skip = true;
                    break;
                }
            }

            // If a node with the same position as successor is in the CLOSED list
            // which has a lower f than successor, skip this successor
            if (closed_list.find(&network[successor.coords.first][successor.coords.second]) != closed_list.end()
                && network[successor.coords.first][successor.coords.second].f <= successor.f)
            {
                skip = true;
            }

            // otherwise, add the node to the open list
            if (!skip)
            {
                open_list.push_back(successor);

                // FIXME: Node should be added using insertion sort. Inefficient repeated sorting of open list
                sort(open_list.begin(), open_list.end(), greater<MeshNode>());
            }
        }
    }

    // Work backwards from target to start to assemble the final path
    pair<int, int> cur_coords = target_coords;
    vector<pair<int, int> > path_coords;

    while (cur_coords != start_coords)
    {
        path.push_back(network[cur_coords.first][cur_coords.second].pos);
        path_coords.push_back(cur_coords);
        cur_coords = network[cur_coords.first][cur_coords.second].parent;
    }

    return path;
}

// Returns a vector of coordinate pairs representing the coordinates of each
// node contained within the object
vector<pair<int, int> > Mesh::getNodesInObject(const GameObject *object) const
{
    vector<pair<int, int> > coordinates;
    Vector3 center = object->getPos();

    Vector3 corner1 = center;
    corner1.x -= object->getDimsX() / 2;
    corner1.y -= object->getDimsY() / 2;

    Vector3 corner2 = center;
    corner2.x += object->getDimsX() / 2;
    corner2.y += object->getDimsY() / 2;

    pair<int, int> start_indices = getIndicesFromPos(corner1);
    pair<int, int> end_indices = getIndicesFromPos(corner2);

    for (int i = start_indices.first; i < end_indices.first; i++)
    {
        for (int j = start_indices.second; j < end_indices.second; j++)
        {
            coordinates.push_back(pair<int, int>(i, j));
        }
    }

    return coordinates;
}

// Returns a coordinate pair representing the coordinates of the node
// nearest to teh supplied position. The supplied position may be
// off the mesh
pair<int, int> Mesh::getIndicesFromPos(Vector3 pos) const
{
    // Round to nearest node
    int x = (pos.x - start_x + tiling / 2) / tiling;
    int y = (pos.y - start_y + tiling / 2) / tiling;

    return pair<int, int>(x, y);
}

// Returns a Vector3 representing the in game location of the node
// at the specified coordinates.
Vector3 Mesh::getPosFromIndices(std::pair<int, int> coords) const
{
    float x = coords.first * tiling + start_x;
    float y = coords.second * tiling + start_y;
    return Vector3(x, y, height);
}

// Returns coordinates of each node adjacent to the supplied coordinate
vector<pair<int, int> > Mesh::getNeighbors(pair<int, int> center) const
{
    vector<pair<int, int> > neighbors;

    pair<int, int> cards[8];
    cards[0] = center; cards[0].second--;
    cards[1] = center; cards[1].first++; cards[1].second--;
    cards[2] = center; cards[2].first++;
    cards[3] = center; cards[3].first++; cards[3].second++;
    cards[4] = center; cards[4].second++;
    cards[5] = center; cards[5].first--; cards[5].second++;
    cards[6] = center; cards[6].first--;
    cards[7] = center; cards[7].first--; cards[7].second--;

    for (int i = 0; i < 8; i++)
    {
        if (inBounds(cards[i].first, cards[i].second))
        {
            neighbors.push_back(cards[i]);
        }
    }

    return neighbors;
}

// Returns true if x and y are valid indices of the network vector
bool Mesh::inBounds(int x, int y) const
{
    return (x >= 0 && x < length)
        && (y >= 0 && y < thickness);
}