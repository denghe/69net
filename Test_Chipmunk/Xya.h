#ifndef __XYA_H__
#define __XYA_H__


// 填充查表用结构
struct Xya
{
    signed char x;
    signed char y;
    signed char a;
};
extern float _xyam;
void fillxya();
Xya* getxya( Point const& a, Point const& b );


#endif
