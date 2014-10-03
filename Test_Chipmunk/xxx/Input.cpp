#include "Precompile.h"

namespace xxx
{
    Input::Input()
    {
        G::input = this;
    }
    Input::~Input()
    {
        G::input = nullptr;
    }
}
