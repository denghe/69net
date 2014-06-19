#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
namespace Lib3 {
namespace Environment {




// 部分代码参考自 papi
INLINE UInt64 GetTickCount() {
#ifdef __linux
#ifdef __IA
		UInt64 ret = 0;
	#ifdef __X64
		do {
			UInt32 a, d;
			asm volatile ( "rdtsc":"=a" ( a ), "=d"( d ) );
			ret = (UInt64)a | ((UInt64)d << 32);
		} while (0);
	#else
		__asm__ __volatile__( "rdtsc":"=A"( ret ): );
	#endif
		return ret;
#else
	timeval tv;
	gettimeofday(&tv, Null);
	return Int64(tv.tv_sec) * 1000000 + tv.tv_usec;
#endif
#else
    LARGE_INTEGER n;
    QueryPerformanceCounter(&n);
    return n.QuadPart;
#endif
}

// linux 下面 需要查找 CPU 硬件信息或者 测试 (这里实现的是　测试　法)
// (取time1,取tick1,sleep(1),取tick2,取time2,算 time1,tick1 与 time2,tick2 的差值做除法)
// (测试法只适合少量调用, 毕竟会卡几十分之一秒。比如可以放在程序刚开始运行的时候取一次, 或第一次获取)
// 注：该计算于 linux 下 结果不是很精确, 先将就用
INLINE UInt64 GetFrequency() {
#ifdef __linux
	timeval t1, t2;
	var tick1 = GetTickCount();
	gettimeofday(&t1, Null);
    usleep(1);
    var tick2 = GetTickCount();
	gettimeofday(&t2, Null);
	var usec = (UInt64(t2.tv_sec) * 1000000 + t2.tv_usec) - (UInt64(t1.tv_sec) * 1000000 + t1.tv_usec);
	var ticks = tick2 - tick1;
	return ticks * 1000 / usec;
#else
    LARGE_INTEGER i;
    QueryPerformanceFrequency(&i);
    return i.QuadPart;
#endif
}





}}
#endif
