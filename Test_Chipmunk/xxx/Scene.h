#ifndef __SCENE_H__
#define __SCENE_H__

namespace xxx
{
    class Scene : public Node
    {
    public:
        Size designSize;

        Scene();
        ~Scene();
        void Draw( int _durationTicks ) override;
    };
}

#endif
