#ifndef __ALL_H__
#define __ALL_H__

// 这是预编译头

#include "cocos2d.h"
#include <algorithm>

using namespace std;
using namespace cocos2d;

/* Common types */
#include <stdint.h>
typedef int8_t  int8, sbyte;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8, byte;
typedef uint16_t uint16, ushort;
typedef uint32_t uint32, uint;
typedef uint64_t uint64;

typedef unsigned long ulong;    // x64: vc 32bit, gcc 64bit
typedef wchar_t wchar;          // win/ios: 16bit, linux, android 32bit

#include "Utils.h"
#include "AppDelegate.h"
#include "Ball.h"
#include "Desk.h"
#include "Monster.h"

#endif
