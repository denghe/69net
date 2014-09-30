#include "Precompile.h"


Audio::Audio()
{
    G::audio = this;
}


Audio::~Audio()
{
    G::audio = nullptr;
}
