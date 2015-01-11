#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "Common.h"

#include <vector>
using namespace std;

/**
 * @class PQNode
 *
 * @brief used to transform class NODE into PriorityQueue Node
 *        user should inherit this class to construct concrete PriorityQueue node
 */
template<typename NODE>
class PQNode
{
protected:
    PQNode()
        : _pq_index(0)
    {}

public:
    virtual ~PQNode() {}

public:
    /**
     * comparer of PriorityQueue
     * >node return 1
     * =node return 0
     * <node return -1
     */
    virtual int compare(const NODE *node) const = 0;

public:
    /// the array index on priority queue
    uint32 _pq_index;
};

/**
 * @class PriorityQueue
 *
 * @brief Priority Queue for fast indexing
 */
template<typename NODE>
class PriorityQueue
{   
public:
    PriorityQueue() {}

    ~PriorityQueue() {}

public:
    /// push a node
    void push(NODE *node);

    /// pop a node
    NODE *pop();

    /// remove a specific node
    void remove(NODE *node);

    /// return top node
    inline NODE *top()
    {
        return _data.front();
    }

    /// adjust modified node to make heap valid
    inline void adjust_node(NODE *node)
    {
        adjust_node(node->_pq_index);
    }

    /// return NODE count
    inline uint32 size() const
    {
        return (uint32)_data.size();
    }

    /// return if PQ is empty
    inline int empty() const
    {
        return _data.empty();
    }

    /// clear PQ NODEs
    inline void clear()
    {
        _data.clear();
    }

private:
    /// core adjust_node method
    void adjust_node(uint32 index);

    /// core PQ method
    void percolate_up(uint32 index);

    /// core PQ method
    void percolate_down(uint32 index);

private:
    vector<NODE *> _data;
};

#include "PriorityQueue.cpp"

#endif
