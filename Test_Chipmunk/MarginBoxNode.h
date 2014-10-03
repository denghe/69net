#ifndef __MARGINBOXNODE_H__
#define __MARGINBOXNODE_H__

struct Margin
{
    float left, top, right, bottom;
};

struct MarginBoxNode : public BoxNode
{
    Margin margin;
    MarginBoxNode();
    void Draw( int durationTicks ) override;
    inline static MarginBoxNode* Create() { return new MarginBoxNode(); }
};


#endif
