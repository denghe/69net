#ifndef __GAME1_H__
#define __GAME1_H__

class Game1 : public Game
{
public:
    Game1();
    ~Game1();
    void Update() override;
protected:
    Audio audio;
    Input input;
    Scene scene;
};

#endif
