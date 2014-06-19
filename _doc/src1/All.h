#ifndef ALL_H
#define ALL_H

#ifdef __linux
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#else
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>
#include <Windows.h>
#include <conio.h>
#endif

#include <assert.h>
#include <locale.h>
#include <math.h>
#include <cmath>
#include <new>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <utility>
#include <typeinfo>
#include <exception>
#include <memory>
#include <algorithm>
#include <ctime>
#include <cstring>


#include "H/BaseDefines.h"
#include "H/Environment.h"
#include "H/Mutex.h"
#include "H/Interlocked.h"
#include "H/NumBits.h"
#include "H/Memory.h"
#include "H/GC_Base.h"
#include "H/GC_CRT.h"
#include "H/GC_Buddy.h"
#include "H/GC_LinkedBlock.h"
#include "H/GC_Pool.h"
#include "H/DateTime.h"
#include "H/Decimal.h"
#include "H/Guid.h"
#include "H/Random.h"
#include "H/Converts.h"
#include "H/Nullable.h"
#include "H/Strings.h"
#include "H/SString.h"
#include "H/String.h"
#include "H/Math.h"
#include "H/Array.h"
#include "H/BitArray.h"
#include "H/List.h"
#include "H/Dictionary.h"
#include "H/LinkedList.h"
#include "H/Binary.h"
#include "H/DbSet.h"
#include "H/BinaryFormatter_Normal.h"
#include "H/Console.h"
#include "H/Cout.h"
#include "H/Stopwatch.h"


#include "HPP/DateTime.hpp"
#include "HPP/Decimal.hpp"
#include "HPP/Guid.hpp"
#include "HPP/Random.hpp"
#include "HPP/Converts.hpp"
#include "HPP/Environment.hpp"
#include "HPP/Nullable.hpp"
#include "HPP/Strings.hpp"
#include "HPP/Console.hpp"
#include "HPP/Cout.hpp"
#include "HPP/GC_CRT.hpp"
#include "HPP/GC_Buddy.hpp"
#include "HPP/GC_LinkedBlock.hpp"
#include "HPP/GC_Pool.hpp"
#include "HPP/Array.hpp"
#include "HPP/SString.hpp"
#include "HPP/String.hpp"
#include "HPP/BitArray.hpp"
#include "HPP/List.hpp"
#include "HPP/Dictionary.hpp"
#include "HPP/LinkedList.hpp"
#include "HPP/Binary.hpp"
#include "HPP/Math.hpp"
#include "HPP/DbSet.hpp"
#include "HPP/Memory.hpp"
#include "HPP/BinaryFormatter_Normal.hpp"


#endif