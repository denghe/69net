#include "Precompile.h"

namespace xxx
{
    Game::Game()
    {
        G::game = this;
    }

    Game::~Game()
    {
        G::game = nullptr;
    }
}
