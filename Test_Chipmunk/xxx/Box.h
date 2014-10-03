#ifndef __BOXNODE_H__
#define __BOXNODE_H__

namespace xxx
{
    struct Box : public Node
    {
        STATIC_CREATE( Box );

        void Draw( int _durationTicks ) override;
    };

}

#endif
