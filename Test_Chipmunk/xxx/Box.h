#ifndef __BOXNODE_H__
#define __BOXNODE_H__

namespace xxx
{
    struct Box : public Node
    {
        float vertexs[ 8 ];
        //GLubyte vertexs_indexs[ 4 ];

        void Drawing( int _durationTicks ) override;
    };

}

#endif
