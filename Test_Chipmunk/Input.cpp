#include "Precompile.h"

Input::Input()
{
    G::input = this;
}


Input::~Input()
{
    G::input = nullptr;
}

void Input::Update( int frameCounter )
{

}
