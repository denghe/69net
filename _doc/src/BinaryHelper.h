#ifndef _BINARYHELPER_H_
#define _BINARYHELPER_H_

// 从数据流读取的几种结果
enum class ReadStatus
{
    Success,             // 成功
    NotEnoughData,       // 剩余数据不足
    Overflow,            // 长度/值 超限，超长
    Fail,                // 各种失败
};

namespace BinaryHelper
{

    inline void dumpAscII( String & s, char const * buf, int len ) 
    {
        for( int i = 0; i < len; ++i )
        {
            uint8 c = buf[ i ];
            if( c < 32 || c > 126 ) s.write( '.' );
            else s.write( (char)c );
        }
    }

    // write buf's binary dump text to s
    inline void dump( String & s, char const * buf, int len )
    {
        if( len == 0 ) return;
        s.write( "\n--------  0  1  2  3 | 4  5  6  7 | 8  9  A  B | C  D  E  F" );
        int i = 0;
        for( ; i < len; ++i )
        {
            if( (i % 16) == 0 )
            {
                if( i )
                {           // output ascii to the end of the line
                    s.write( "  " );
                    dumpAscII( s, buf + i - 16, 16 );
                }
                s.write( '\n' );
                s.ensure( 8 );
                s._len += Converts::toStringHex( s._buf + s._len, i );
                s.write( "  " );
            }
            else if( i && (i % 4 == 0) )
            {
                s.write( "  " );
            }
            else s.write( ' ' );
            s.ensure( 2 );
            s._len += Converts::toStringHex( s._buf + s._len, (uint8)buf[ i ] );
        }
        int left = i % 16;
        if( left )
        {
            len = len + 16 - left;
            for( ; i < len; ++i )
            {
                if( i && (i % 4 == 0) )
                    s.write( "  " );
                else s.write( ' ' );
                s.write( "  " );
            }
            s.write( "  " );
            dumpAscII( s, buf + i - 16, left );
        }
    }

    // set: direct write to buf[ offset ], dont change offset's value

    INLINE void set( char * buf, int offset, char const * v, int len )
    {
        memcpy( buf + offset, v, len );
    }

    template<typename T>
    void set( char * const & buf, int const & offset, T const & v )
    {
#ifdef __IA
        *(T*)( buf + offset ) = v;
#else
        auto b = buf + offset;
        auto p = (char*)&v;
        if( sizeof(v) >= 1  ) {
            b[ 0 ] = p[ 0 ];
        }
        if( sizeof(v) >= 2 ) {
            b[ 1 ] = p[ 1 ];
        }
        if( sizeof(v) >= 4 ) {
            b[ 2 ] = p[ 2 ];
            b[ 3 ] = p[ 3 ];
        }
        if( sizeof(v) == 8 ) {
            b[ 4 ] = p[ 4 ];
            b[ 5 ] = p[ 5 ];
            b[ 6 ] = p[ 6 ];
            b[ 7 ] = p[ 7 ];
        }
#endif
    }

    // write: write v to buf[ offset ], offset += v's byte count

    INLINE void write( char * buf, int & offset, char const * v, int len )
    {
        memcpy( buf + offset, v, len );
        offset += len;
    }

    template<typename T>
    void write( char * buf, int & offset, T const & v )
    {
        set( buf, offset, v );
        offset += sizeof(v);
    }


    // get: get v's value from buf[ offset ]

    INLINE void get( char * v, char const * buf, int offset, int len )
    {
        memcpy( v, buf + offset, len );
    }

    template<typename T>
    void get( T & v, char const * buf, int offset )
    {
#ifdef __IA
        v = *(T*)( buf + offset );
#else
        auto b = buf + offset;
        auto p = (char*)&v;
        if( sizeof(v) >= 1 ) {
            p[ 0 ] = b[ 0 ];
        }
        if( sizeof(v) >= 2 ) {
            p[ 1 ] = b[ 1 ];
        }
        if( sizeof(v) >= 4 ) {
            p[ 2 ] = b[ 2 ];
            p[ 3 ] = b[ 3 ];
        }
        if( sizeof(v) >= 8 ) {
            p[ 4 ] = b[ 4 ];
            p[ 5 ] = b[ 5 ];
            p[ 6 ] = b[ 6 ];
            p[ 7 ] = b[ 7 ];
        }
#endif
    }

    // read: get v's value from buf[ offset ], offset += v's byte count

    INLINE void read( char * v, char const * buf, int & offset, int len )
    {
        memcpy( v, buf + offset, len );
        offset += len;
    }

    template<typename T>
    void read( T & v, char const * buf, int & offset )
    {
        get( v, buf, offset );
        offset += sizeof(v);
    }












    // negative -> ZegZag positive
    INLINE uint16 zegZagEncode( int16  i ) { return ( i << 1 ) ^ ( i >> 15 ); }
    INLINE uint32 zegZagEncode( int32  i ) { return ( i << 1 ) ^ ( i >> 31 ); }
    INLINE uint64 zegZagEncode( int64  i ) { return ( i << 1 ) ^ ( i >> 63 ); }

    // ZegZag positive -> negative
    INLINE int16  zegZagDecode( uint16 i ) { return (int16)( i >> 1 ) ^ ( -(int16)( i & 1 ) ); }
    INLINE int32  zegZagDecode( uint32 i ) { return (int32)( i >> 1 ) ^ ( -(int32)( i & 1 ) ); }
    INLINE int64  zegZagDecode( uint64 i ) { return (int64)( i >> 1 ) ^ ( -(int64)( i & 1 ) ); }


    // need ensure 7
    inline void writeDirect732( char * buf, int & offset, uint32 v )
    {
Lab1:
        auto b7 = uint8(v);
        if(( v >>= 7 ))
        {
            buf[ offset++ ] = b7 | 0x80u;
            goto Lab1;
        }
        buf[ offset++ ] = b7;
    }

    // to 7bit varlen. need ensure 9
    inline void writeDirect764( char * buf, int & offset, uint64 v )
    {
        auto idx8 = offset + 8;
Lab1:
        auto b7 = uint8(v);
        if(( v >>= 7 ))
        {
            buf[ offset++ ] = b7 | 0x80u;
            if( offset == idx8 )
                buf[ offset++ ] = (uint8)v;
            else
                goto Lab1;
        }
        else
        {
            buf[ offset++ ] = b7;
        }
    }

    inline void readDirect764( uint64 & v, char * buf, int & offset )
    {
        auto p = buf + offset;
        uint64 i = 0, b7;
        uint lshift = 0, idx = 0;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            if( idx < 8 )
            {
                i |= ( b7 & 0x7F ) << lshift;
                lshift += 7;
                goto Lab1;
            }
            else i |= ( b7 << lshift ) | ( (uint64)p[ idx++ ] << 28 << 28 );
        } 
        else i |= b7 << lshift;
        v = i;
        offset += idx;
    }

    inline void readDirect732( uint32 & v, char * buf, int & offset )
    {
        auto p = buf + offset;
        uint i = 0, b7, lshift = 0, idx = 0;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            i |= ( b7 & 0x7F ) << lshift;
            lshift += 7;
            goto Lab1;
        }
        else i |= b7 << lshift;
        v = i;
        offset += idx;
    }

    inline void readDirect716( uint16 & v, char * buf, int & offset )
    {
        auto p = buf + offset;
        uint i = 0, b7, lshift = 0, idx = 0;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            i |= ( b7 & 0x7F ) << lshift;
            lshift += 7;
            goto Lab1;
        }
        else i |= b7 << lshift;
        v = (uint16)i;
        offset += idx;
    }


    inline ReadStatus read764( uint64 & v, char * _buf, int _len, int & _readIdx )
    {
        if( _readIdx >= _len ) return ReadStatus::NotEnoughData;
        auto p = _buf + _readIdx;
        uint64 i = 0, b7;
        uint lshift = 0, idx = 0, len = _len - _readIdx;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            if( idx >= len ) return ReadStatus::NotEnoughData;
            if( idx < 8 )
            {
                i |= ( b7 & 0x7F ) << lshift;
                lshift += 7;
                goto Lab1;
            }
            else i |= ( b7 << lshift ) | ( (uint64)p[ idx++ ] << 28 << 28 );
        } 
        else i |= b7 << lshift;
        v = i;
        _readIdx += idx;
        return ReadStatus::Success;
    }

    inline ReadStatus read732( uint32 & v, char * _buf, int _len, int & _readIdx )
    {
        if( _readIdx >= _len ) return ReadStatus::NotEnoughData;
        auto p = _buf + _readIdx;
        uint i = 0, b7, lshift = 0, idx = 0, len = _len - _readIdx;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            if( idx == 5 ) return ReadStatus::Overflow;
            if( idx >= len ) return ReadStatus::NotEnoughData;
            i |= ( b7 & 0x7F ) << lshift;
            lshift += 7;
            goto Lab1;
        }
        else
        {
            if( idx == 5 && b7 > 15 ) return ReadStatus::Overflow;
            else i |= b7 << lshift;
        }
        v = i;
        _readIdx += idx;
        return ReadStatus::Success;
    }

    inline ReadStatus read716( uint16 & v, char * _buf, int _len, int & _readIdx )
    {
        if( _readIdx >= _len ) return ReadStatus::NotEnoughData;
        auto p = _buf + _readIdx;
        uint i = 0, b7, lshift = 0, idx = 0, len = _len - _readIdx;
Lab1:
        b7 = p[ idx++ ];
        if( b7 > 0x7F )
        {
            if( idx == 3 ) return ReadStatus::Overflow;
            if( idx >= len ) return ReadStatus::NotEnoughData;
            i |= ( b7 & 0x7F ) << lshift;
            lshift += 7;
            goto Lab1;
        }
        else
        {
            if( idx == 3 && b7 > 3 ) return ReadStatus::Overflow;
            else i |= b7 << lshift;
        }
        v = (uint16)i;
        _readIdx += idx;
        return ReadStatus::Success;
    }




}

#endif
