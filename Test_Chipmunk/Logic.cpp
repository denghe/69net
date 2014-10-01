#include "Precompile.h"

Logic::Logic()
{
    G::logic = this;
}

Logic::~Logic()
{
    G::logic = nullptr;
}

