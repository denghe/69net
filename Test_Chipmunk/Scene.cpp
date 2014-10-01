#include "Precompile.h"

Scene::Scene()
{
    G::scene = this;
}

Scene::~Scene()
{
    Node::Clear();
    G::scene = nullptr;
}
