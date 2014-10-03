#include "Precompile.h"

namespace xxx
{
    Game::Game()
    {
        G::logic = this;
    }

    Game::~Game()
    {
        G::logic = nullptr;
    }
}
