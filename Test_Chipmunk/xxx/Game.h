#ifndef __LOGIC_H__
#define __LOGIC_H__

namespace xxx
{
    class Game
    {
    public:
        Game();
        virtual ~Game();
        virtual void Loaded() = 0;
        virtual void Update() = 0;
    };
}

#endif
