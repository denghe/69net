#ifndef __GAME1_H__
#define __GAME1_H__

class Node1 : public Node
{
public:
    inline static Node1* create() { return new Node1(); }
    void Draw( int durationTicks ) override;
};

class Game1 : public Logic
{
public:
    Game1();
    ~Game1();
    void Update() override;
protected:
    Scene scene;
};

#endif
