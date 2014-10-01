#include "Precompile.h"


namespace xxx
{
    Audio::Audio()
    {
        G::audio = this;
    }


    Audio::~Audio()
    {
        G::audio = nullptr;
    }
}
