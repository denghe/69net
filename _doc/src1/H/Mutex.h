#ifndef MUTEX_H
#define MUTEX_H
namespace Lib3 {

class Mutex
{
private:
  void operator=(Mutex &m) {}
  Mutex( const Mutex &m) {}
#ifdef __linux
  mutable pthread_mutex_t _m;
public:
  INLINE Mutex() {
    pthread_mutexattr_t t;
    pthread_mutexattr_init(&t);
    pthread_mutexattr_settype(&t, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_m, &t);
    pthread_mutexattr_destroy(&t);
  }
  INLINE ~Mutex() {
      pthread_mutex_unlock(&_m);
      pthread_mutex_destroy(&_m);
  }

  INLINE void Lock() const {
      pthread_mutex_lock(&_m);
  }
  INLINE Boolean TryLock() const {
      return pthread_mutex_trylock(&_m) > 0;
  }
  INLINE void Unlock() const {
      pthread_mutex_unlock(&_m);
  }
#else
  mutable CRITICAL_SECTION _m;
public:
  INLINE Mutex() {
      InitializeCriticalSection(&_m);
  }
  INLINE ~Mutex() {
      DeleteCriticalSection(&_m);
  }
  INLINE void Lock() const {
      EnterCriticalSection(&_m);
  }
#if(_WIN32_WINNT >= 0x0400)
  INLINE Boolean TryLock() const {
      return TryEnterCriticalSection(&_m) > 0;
  }
#endif
  INLINE void Unlock() const {
      LeaveCriticalSection(&_m);
  }
#endif
};

}
#endif
