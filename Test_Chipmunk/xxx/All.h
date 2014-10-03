#ifndef __ALLXXX_H__
#define __ALLXXX_H__

#define STATIC_CREATE( T ) \
inline static T* Create()  \
{                          \
    auto rtv = new T();    \
    rtv->AutoRelease();    \
    return rtv;            \
}


#include "Ref.h"
#include "Window.h"
#include "Input.h"
#include "Audio.h"
#include "Node.h"
#include "Box.h"
#include "Scene.h"
#include "Game.h"
#include "Looper.h"
#include "G.h"

#endif
