#ifndef __BOXNODE_H__
#define __BOXNODE_H__

namespace xxx
{
    struct Box : public Node
    {
        void Draw( int durationTicks ) override;
        static Box* Create() { return new Box(); }
    };

}

#endif
