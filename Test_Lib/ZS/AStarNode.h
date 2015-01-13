#ifndef _ASTAR_NODE_H_
#define _ASTAR_NODE_H_

#include "PriorityQueue.h"

#include <unordered_set>
using namespace std;

/**
 * @class AStarNode
 * @brief used to transform class NODE into AStarNode
 *
 * user should inherit this class to construct concrete A* node
 */
template<typename NODE>
class AStarNode : public PQNode<NODE>
{
protected:
    AStarNode()
        : _key(0)
        , _g(0)
        , _h(0)
        , _on_open(false)
        , _parent(0)
    {}

public:
    virtual ~AStarNode() {}

/// overridable
public:
    /// function to calculate node key
    virtual uint32 key() = 0;

    /// function to return f value
    virtual float f() const
    {
        return _g + _h;
    }

public:
    /// return parent node
    inline NODE *parent() const
    {
        return _parent;
    }

    /// PQ Node comparer
    inline int compare(const NODE *node) const
    {
        if ( this->f() > node->f() )
            return 1;
        else if ( this->f() < node->f() )
            return -1;
        else
            return 0;
    }

public:
    /// key must be unqiue for each node
    uint32 _key;

    /// cost from start to self
    float _g;

    /// estimate from self to goal
    float _h;

    /// indicate if the node is on the open list
    bool _on_open;

    /// link to its parent node. this is used to keep track of the searched path
    NODE *_parent;
};


/**
 * @class AStarNodeHash
 * @brief used for AStarNode hashing
 */
template<typename NODE>
class AStarNodeHash
{
public:
    size_t operator()(const NODE *node) const
    {
        return std::hash<uint32>()(node->_key);
    }

    bool operator()(const NODE *a, const NODE *b) const
    {
        return a->_key == b->_key;
    }
};

#endif
