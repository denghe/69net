#include "Precompile.h"

namespace xxx
{
    Scene::Scene()
    {
        G::scene = this;
    }

    Scene::~Scene()
    {
        NodeBase::Clear();
        G::scene = nullptr;
    }
}