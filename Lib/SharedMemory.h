#pragma once

namespace xxx
{
    class SharedMemory
    {
    public:
        static char* Create( int key, int bufLen );
        static int Dispose( int key );
        static char* Get( int key, int bufLen );

        template<typename T>
        static void SetPrefixName( T&& s );
    private:
        static String prefixName;
    };
}
