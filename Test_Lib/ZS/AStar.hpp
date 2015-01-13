#ifndef _ASTAR_HPP_
#define _ASTAR_HPP_

template<typename NODE, typename WAYPOINT>
void AStar<NODE, WAYPOINT>::init(NODE *start, NODE *goal, void *user_data)
{
    // set goal node
    _goal = goal;
    _goal->_key = _goal->key();

    // push start node
    init_node(start, 0, 0);
    add_to_open(start);

    // set user data
    _user_data = user_data;
}

template<typename NODE, typename WAYPOINT>
int AStar<NODE, WAYPOINT>::search(NODE **searching_node)
{
    if (_open.empty()) return -1;

    int r;

    // pop the node with smallest f
    NODE *node = _open.pop();

    // check if goal is reached
    if ( is_goal(node) )
    {
        if (_path) delete _path;
        _path = build_path(node);
        r = 1;
    }
    else
    {
        // expand node
        expand_node(node);

        // process successors
        NODE *successor;
        while ( !_successors.empty() )
        {
            // get one successor
            successor = _successors.front();
            _successors.pop_front();

            // process it
            typename NodeSet::iterator it = _node_table.find(successor);
            if (it != _node_table.end())
            {
                NODE *found_node = *it;

                if (found_node->_on_open) // on open list
                {
                    if (successor->f() < found_node->f()) // successor is better
                    {
                        // update found_node
                        found_node->_g      = successor->_g;
                        found_node->_h      = successor->_h;
                        found_node->_parent = node;

                        // adjust found_node to keep heap properties
                        _open.adjust_node(found_node);
                    }
                }
                else // on closed list
                {
                    if (successor->f() < found_node->f()) // successor is better
                    {
                        // update found_node
                        found_node->_g      = successor->_g;
                        found_node->_h      = successor->_h;
                        found_node->_parent = node;

                        // add to open
                        add_to_open(found_node, 0);
                    }
                }

                // discard successor
                delete successor;
            }
            else // on either list
            {
                // set parent
                successor->_parent = node;

                // add to open
                add_to_open(successor);
            }
        }

        r = 0;
    }

    // add parent node to closed
    add_to_closed(node);

    // set searching node
    if (searching_node) *searching_node = node;

    return r;
}

template<typename NODE, typename WAYPOINT>
typename AStar<NODE, WAYPOINT>::WayPointList *AStar<NODE, WAYPOINT>::detach_path()
{
    WayPointList *p = _path;
    _path = 0;

    return p;
}

template<typename NODE, typename WAYPOINT>
void AStar<NODE, WAYPOINT>::reset()
{
    // clear open list
    _open.clear();

    // clear node table
    for (typename NodeSet::iterator it = _node_table.begin();
        it != _node_table.end();
        ++it)
    {
        delete *it;
    }
    _node_table.clear();

    // clear goal node
    if (_goal) SAFE_DELETE(_goal);

    // clear path
    if (_path) SAFE_DELETE(_path);
}

template<typename NODE, typename WAYPOINT>
void AStar<NODE, WAYPOINT>::add_successor(NODE *node, NODE *parent, float parent_self_cost)
{
    init_node(node, parent ? parent->_g : 0, parent_self_cost);
    _successors.push_back(node);
}

template<typename NODE, typename WAYPOINT>
void AStar<NODE, WAYPOINT>::init_node(NODE *node, float parent_cost, float parent_self_cost)
{
    node->_key = node->key();
    node->_g   = parent_cost + parent_self_cost;
    node->_h   = h(node, _goal);
}

template<typename NODE, typename WAYPOINT>
void AStar<NODE, WAYPOINT>::add_to_open(NODE *node, int add_to_node_table)
{
    _open.push(node);
    if (add_to_node_table) _node_table.insert(node);
    node->_on_open = 1;
}

#endif
