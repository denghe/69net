#ifndef STOPWATCH_H
#define STOPWATCH_H
namespace Lib3 {


struct Stopwatch {
    Stopwatch();
    Stopwatch(UInt64 const &times, void (*f) (), Char const * const &msgHead);
    void Start();
    void Stop();
    void StopAndShow();
    UInt64 ElapsedMilliumSeconds();
    void Show();
    void Show(Char const * const &msgHead);
    void Test(UInt64 const &times, void (*f) (), Char const * const &msgHead);

private:
#ifdef __linux
    UInt64 _begin, _end, _frequency;
#else
    LARGE_INTEGER _begin, _end, _frequency;
#endif
};




#ifdef __linux
INLINE Stopwatch::Stopwatch() {
    _begin = _end = 0;
    _frequency = 1000000;
}

INLINE Stopwatch::Stopwatch(UInt64 const &times, void (*f) (), Char const * const &msgHead) {
    _begin = _end = 0;
    _frequency = 1000000;
    Test(times, f, msgHead);
}

INLINE void Stopwatch::Start() {
    timeval start;
    gettimeofday(&start, NULL);
    _begin = start.tv_sec * _frequency + start.tv_usec;
}

INLINE void Stopwatch::Stop() {
    timeval end;
    gettimeofday(&end, NULL);
    _end = end.tv_sec * _frequency + end.tv_usec;
}

INLINE UInt64 Stopwatch::ElapsedMilliumSeconds() {
    return UInt64((Double)(_end - _begin) / _frequency * 1000);
}
#else

INLINE Stopwatch::Stopwatch() {
    _begin.QuadPart = _end.QuadPart = 0;
    QueryPerformanceFrequency(&_frequency);
}

INLINE Stopwatch::Stopwatch(UInt64 const &times, void (*f) (), Char const * const &msgHead) {
    QueryPerformanceFrequency(&_frequency);
    Test(times, f, msgHead);
}

INLINE void Stopwatch::Start() {
    QueryPerformanceCounter(&_begin);
}

INLINE void Stopwatch::Stop() {
    QueryPerformanceCounter(&_end);
}

INLINE UInt64 Stopwatch::ElapsedMilliumSeconds() {
    return UInt64((Double)(_end.QuadPart - _begin.QuadPart) / _frequency.QuadPart * 1000);
}

#endif
    
INLINE void Stopwatch::Show() {
    Cout << "Elapsed ms: " << ElapsedMilliumSeconds();
}

INLINE void Stopwatch::Show(Char const * const &msgHead) {
    Cout << msgHead << ": Elapsed ms: " << ElapsedMilliumSeconds();
}

INLINE void Stopwatch::Test(UInt64 const &times, void (*f) (), Char const * const &msgHead) {
    Start();
    for (UInt64 i = 0; i < times; ++i) f();
    Stop();
    Cout << msgHead << ": Times: " << times << ", Elapsed ms: " << ElapsedMilliumSeconds() <<"\n";
    Show(msgHead);
}

INLINE void Stopwatch::StopAndShow() {
    Stop();
    Show();
    Console::WriteChar('\n');
}



}
#endif