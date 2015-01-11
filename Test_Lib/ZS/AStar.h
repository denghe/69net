#ifndef _ASTAR_H_
#define _ASTAR_H_

#include "../core/Common.h"

#include <list>
using namespace std;

#include "AStarNode.h"

/**
 * @class AStar
 *
 * user should inherit this class to construct concrete A*
 *
 * NODE:     should be AStartNode
 * WAYPOINT: waypoint class
 */
template<typename NODE, typename WAYPOINT>
class AStar
{
public:
    typedef list<WAYPOINT> WayPointList;

protected:
    AStar()
        : _goal(0)
        , _path(0)
        , _user_data(0)
    {}

public:
    virtual ~AStar()
    {
        reset();
    }

public:
    /// init A*
    void init(NODE *start, NODE *goal, void *user_data = 0);

    /**
     * search a path from start to goal in steps
     *
     * @param searching_node: return current searching node if requested
     *
     * @retval -1 no path found
     * @retval  0 search should be continued
     * @retval  1 a path is found
     */
    int search(NODE **searching_node = 0);

    /**
     * detach the found path
     * the waypoint list should be freed by user
     */
    WayPointList *detach_path();

    /// reset A* to its initial state
    void reset();

    /// return goal node
    inline NODE *goal() const
    {
        return _goal;
    }

    /// get user data
    inline void *user_data() const
    {
        return _user_data;
    }

/// overridable
protected:
    /// determine if the node is goal
    virtual bool is_goal(NODE *node)
    {
        return node->_key == _goal->_key;
    }

    /// function to estimate h from node to goal
    virtual float h(NODE *node, NODE *goal) = 0;

    /**
     * function to expand parent node and produce successors.
     * subclass should use add_successor() to add successors
     */
    virtual void expand_node(NODE *node) = 0;

    /**
     * build a waypoint list from "node"
     * subclass should allocate a new waypoint list
     * the waypoint list should be freed by the caller
     */
    virtual WayPointList *build_path(NODE *node) = 0;

protected:
    /**
     * add a successor to A*
     *
     * @param node:             the node that is to be added
     * @param parent:           parent node of "node"
     * @param parent_self_cost: the cost from parent to "node"
     */
    void add_successor(NODE *node, NODE *parent, float parent_self_cost);

private:
    /// init a node
    void init_node(NODE *node, float parent_cost, float parent_self_cost);

    void add_to_open(NODE *node, int add_to_node_table = 1);

    inline void add_to_closed(NODE *node)
    {
        node->_on_open = false;
    }

private:
    /// open list
    PriorityQueue<NODE> _open;

    /// node table. managing all nodes on open and closed
    typedef unordered_set<NODE *, AStarNodeHash<NODE>, AStarNodeHash<NODE> > NodeSet;
    NodeSet _node_table;

    /// successor list
    typedef list<NODE *> SuccessorList;
    SuccessorList _successors;

    /// goal node
    NODE *_goal;

    /// waypoint list
    WayPointList *_path;

    /// user data
    void *_user_data;
};

#include "AStar.cpp"

#endif
