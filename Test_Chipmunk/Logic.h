#ifndef __LOGIC_H__
#define __LOGIC_H__

class Looper;
class Logic
{
    friend Looper;
public:
    Logic( Video* v = nullptr, Input* i = nullptr, Audio* a = nullptr );
    ~Logic();

    void update();
//private:
    int _f = 0;

    Video* _v;
    Input* _i;
    Audio* _a;
};

#endif
