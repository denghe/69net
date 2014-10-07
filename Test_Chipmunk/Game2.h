#ifndef __GAME2_H__
#define __GAME2_H__

class Game2 : public Game
{
public:
    Game2();
    ~Game2();
    void Loaded() override;
    void Update() override;
protected:
    Audio audio;
    Input input;
    Scene scene;

    void createBox( int _x, int _y );
    CdGrid cdgrid;
    List<CdItem*> touchedItems;
};

#endif
