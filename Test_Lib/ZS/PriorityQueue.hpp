#ifndef _PRIORITY_QUEUE_HPP_
#define _PRIORITY_QUEUE_HPP_

#include <assert.h>

template<typename NODE>
inline void PriorityQueue<NODE>::push(NODE *node)
{
    _data.push_back(node);
    percolate_up((uint32)_data.size() - 1);
}

template<typename NODE>
NODE *PriorityQueue<NODE>::pop()
{
    assert(_data.size() > 0);

    NODE *node = _data.front();

    _data[0] = _data.back();
    _data.pop_back();

    if ( !_data.empty() ) percolate_down(0);

    return node;
}

template<typename NODE>
void PriorityQueue<NODE>::remove(NODE *node)
{
    //check index validation
    assert(_data.size() > 0);

    uint32 index = node->_pq_index;
    uint32 n = (uint32)_data.size() - 1;

    assert(index <= n);

    //remove
    if (index == n)
    {
        _data.pop_back();
    }
    else
    {
        _data[index] = _data[n];
        _data.pop_back();
        this->adjust_node(index);
    }
}

template<typename NODE>
void PriorityQueue<NODE>::adjust_node(uint32 index)
{
    assert(index < (uint32)_data.size());

    if (index == 0)
    {
        percolate_down(0);
    }
    else
    {
        if (_data[index]->compare(_data[(index - 1) >> 1]) < 0)
            percolate_up(index);
        else
            percolate_down(index);
    }
}

template<typename NODE>
void PriorityQueue<NODE>::percolate_up(uint32 index)
{
    NODE *node = _data[index];
    uint32 parent;

    while (index > 0)
    {
        parent = (index - 1) >> 1;

        if (node->compare(_data[parent]) >= 0) break;

        _data[index] = _data[parent];
        _data[index]->_pq_index = index;

        index = parent;
    }

    _data[index] = node;
    node->_pq_index = index;
}

template<typename NODE>
void PriorityQueue<NODE>::percolate_down(uint32 index)
{
    NODE *node = _data[index];
    uint32 left,right,child;

    while (1)
    {
        left = (index<<1) + 1;
        right = left + 1;

        if (right < size())
        {
            if (_data[left]->compare(_data[right]) < 0)
                child = left;
            else
                child = right;
        }
        else if (left < size())
        {
            child = left;
        }
        else
        {
            break;
        }

        if (node->compare(_data[child]) <= 0) break;

        _data[index] = _data[child];
        _data[index]->_pq_index = index;

        index = child;
    }

    _data[index] = node;
    node->_pq_index = index;
}

#endif
