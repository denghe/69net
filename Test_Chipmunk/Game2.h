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

    MarginBox* mb;
    Hash<Object*> objs;
    void createObj( float x, float y );
};

#endif
