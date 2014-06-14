#ifndef STOPWATCH_H
#define STOPWATCH_H


struct Stopwatch {
    Stopwatch();
    Stopwatch(uint64 const &times, void (*f) (), wchar_t const * const &msgHead);
    void start();
    void stop();
    void stopAndShow();
    uint64 ElapsedMilliumSeconds();
    void show();
    void show(wchar_t const * const &msgHead);
    void test(uint64 const &times, void (*f) (), wchar_t const * const &msgHead);

private:
#ifdef __GCC
    uint64 _begin, _end, _frequency;
#else
    LARGE_INTEGER _begin, _end, _frequency;
#endif
};




#ifdef __GCC
INLINE Stopwatch::Stopwatch() {
    _begin = _end = 0;
    _frequency = 1000000;
}

INLINE Stopwatch::Stopwatch(uint64 const &times, void (*f) (), wchar_t const * const &msgHead) {
    _begin = _end = 0;
    _frequency = 1000000;
    test(times, f, msgHead);
}

INLINE void Stopwatch::start() {
    timeval start;
    gettimeofday(&start, NULL);
    _begin = start.tv_sec * _frequency + start.tv_usec;
}

INLINE void Stopwatch::stop() {
    timeval end;
    gettimeofday(&end, NULL);
    _end = end.tv_sec * _frequency + end.tv_usec;
}

INLINE uint64 Stopwatch::ElapsedMilliumSeconds() {
    return uint64((double)(_end - _begin) / _frequency * 1000);
}
#else

INLINE Stopwatch::Stopwatch() {
    _begin.QuadPart = _end.QuadPart = 0;
    QueryPerformanceFrequency(&_frequency);
}

INLINE Stopwatch::Stopwatch(uint64 const &times, void (*f) (), wchar_t const * const &msgHead) {
    QueryPerformanceFrequency(&_frequency);
    test(times, f, msgHead);
}

INLINE void Stopwatch::start() {
    QueryPerformanceCounter(&_begin);
}

INLINE void Stopwatch::stop() {
    QueryPerformanceCounter(&_end);
}

INLINE uint64 Stopwatch::ElapsedMilliumSeconds() {
    return uint64((double)(_end.QuadPart - _begin.QuadPart) / _frequency.QuadPart * 1000);
}

#endif
    
INLINE void Stopwatch::show() {
    std::cout << "Elapsed ms: " << ElapsedMilliumSeconds();
}

INLINE void Stopwatch::show(wchar_t const * const &msgHead) {
    std::cout << msgHead << ": Elapsed ms: " << ElapsedMilliumSeconds();
}

INLINE void Stopwatch::test(uint64 const &times, void (*f) (), wchar_t const * const &msgHead) {
    start();
    for (uint64 i = 0; i < times; ++i) f();
    stop();
    std::cout << msgHead << ": Times: " << times << ", Elapsed ms: " << ElapsedMilliumSeconds() <<"\n";
    show(msgHead);
}

INLINE void Stopwatch::stopAndShow() {
    stop();
    show();
    std::cout << "\n";
}


#endif
