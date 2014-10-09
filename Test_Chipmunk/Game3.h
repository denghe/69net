#ifndef __GAME3_H__
#define __GAME3_H__

class Game3 : public Game
{
public:
    Game3();
    ~Game3();
    void Loaded() override;
    void Update() override;
protected:
    Audio audio;
    Input input;
    Scene scene;

    Node* plane;
    CdGrid* cdgrid;
};

#endif
