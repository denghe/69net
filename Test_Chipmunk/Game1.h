#ifndef __GAME1_H__
#define __GAME1_H__

class Game1 : public Game
{
public:
    Game1();
    ~Game1();
    void Loaded() override;
    void Update() override;
protected:
    Audio audio;
    Input input;
    Scene scene;

    MarginBox* mb;
    Set<Object*> objs;
    void createObj( float x, float y );
};

#endif
