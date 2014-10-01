#include "Precompile.h"

namespace xxx
{
    Logic::Logic()
    {
        G::logic = this;
    }

    Logic::~Logic()
    {
        G::logic = nullptr;
    }
}
