#ifndef __GAME4_H__
#define __GAME4_H__

class Game4 : public Game
{
public:
    Game4();
    ~Game4();
    void Loaded() override;
    void Update() override;
protected:
    Audio audio;
    Input input;
    Scene scene;

    CdGrid cdgrid;
};

#endif
