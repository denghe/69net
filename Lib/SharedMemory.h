#ifndef _SHAREDMEMORY_H__
#define _SHAREDMEMORY_H__


class SharedMemory
{
public:
    static char* create( int key, int bufLen );
    static int dispose( int key );
    static char* get( int key, int bufLen );

    template<typename T>
    static void setPrefixName( T&& s );
private:
    static String _prefixName;
};


#endif
