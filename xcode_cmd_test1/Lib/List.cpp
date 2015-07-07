#include "All.h"

namespace xxx
{
    List<bool>::List( int capacity )
    {
        assert( capacity > 0 );
        auto len = int( capacity / 8 );
        if( len < 64 ) len = 64;
        else len = (int)Round2n( len );
        size = 0;
        buf = new char[ len ];
        maxSize = len * 8;
    }

    List<bool>::~List()
    {
        if( buf )
        {
            Clear();
            delete[] buf;
        }
    }

    List<bool>::List( List<bool> && o )
        : buf( o.buf )
        , size( o.size )
        , maxSize( o.maxSize )
    {
        o.buf = nullptr;
    }

    List<bool>::List( List<bool> const & o )
        : List<bool>( o.size )
    {
        memcpy( buf, o.buf, o.ByteSize() );
    }

    List<bool>& List<bool>::operator=( List<bool> && o )
    {
        delete[] buf;
        buf = o.buf;
        size = o.size;
        maxSize = o.maxSize;
        o.buf = nullptr;
        return *this;
    }

    List<bool>& List<bool>::operator=( List<bool> const & o )
    {
        if( this == &o ) return *this;
        size = o.size;
        if( maxSize < o.size )
        {
            auto len = int( o.size / 8 );
            if( len < 64 ) len = 64;
            else len = (int)Round2n( len );
            maxSize = len * 8;
            delete[] buf;
            buf = new char[ len ];
        }
        memcpy( buf, o.buf, o.ByteSize() );
        return *this;
    }

    void List<bool>::Push( bool v )
    {
        if( size == maxSize ) Reserve( size + 1 );
        Set( size++, v );
    }

    void List<bool>::Pop()
    {
        assert( size > 0 );
        --size;
    }

    bool List<bool>::Top() const
    {
        assert( size > 0 );
        return operator[]( size - 1 );
    }

    void List<bool>::Clear()
    {
        size = 0;
    }

    void List<bool>::Reserve( int capacity )
    {
        assert( capacity > 0 );
        if( capacity <= maxSize ) return;
        auto len = (int)Round2n( ( capacity - 1 ) / 8 + 1 );
        maxSize = len * 8;
        auto newBuf = new char[ len ];
        memcpy( newBuf, buf, ByteSize() );
        delete[] buf;
        buf = newBuf;
    }

    char* List<bool>::Data() const
    {
        return buf;
    }

    int List<bool>::Size() const
    {
        return size;
    }

    int List<bool>::ByteSize() const
    {
        if( size ) return ( size - 1 ) / 8 + 1;
        return 0;
    }

    bool List<bool>::operator[]( int idx ) const
    {
        return At( idx );
    }

    bool List<bool>::At( int idx ) const
    {
        return ( ( (size_t*)buf )[ idx / ( sizeof( size_t ) * 8 ) ] &
                 ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) ) ) > 0;
    }

    void List<bool>::Set( int idx, bool v )
    {
        assert( idx >= 0 && idx < size );
        if( v ) SetTrue( idx );
        else SetFalse( idx );
    }

    void List<bool>::SetTrue( int idx )
    {
        assert( idx >= 0 && idx < size );
        ( (size_t*)buf )[ idx / ( sizeof( size_t ) * 8 ) ] |=
            ( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
    }

    void List<bool>::SetFalse( int idx )
    {
        assert( idx >= 0 && idx < size );
        ( (size_t*)buf )[ idx / ( sizeof( size_t ) * 8 ) ] &=
            ~( size_t( 1 ) << ( idx % ( sizeof( size_t ) * 8 ) ) );
    }

    void List<bool>::FillTrue()
    {
        memset( buf, 0xFFFFFFFFu, ( size - 1 ) / 8 + 1 );
    }

    void List<bool>::FillFalse()
    {
        memset( buf, 0, ( size - 1 ) / 8 + 1 );
    }

    void List<bool>::Fill( bool v, int idxFrom, int idxTo )
    {
        assert( size > 0 && idxFrom >= 0 && idxTo >= 0 && idxFrom < size && idxTo < size );
        if( idxFrom == idxTo )
        {
            Set( idxFrom, v );
            return;
        }
        if( idxFrom > idxTo )
        {
            std::swap( idxFrom, idxTo );
        }
        auto byteIdxFrom = idxFrom >> 3;
        auto byteIdxTo = idxTo >> 3;

        if( byteIdxFrom == byteIdxTo )
        {
            // 搞一个 中间一段是 v 的 byte 出来
            if( v )
            {
                buf[ byteIdxFrom ] |= (byte)0xFFu >> ( 7 - ( idxTo - idxFrom ) ) << ( idxFrom & 7 );
            }
            else
            {
                buf[ byteIdxFrom ] &= ~( (byte)0xFFu >> ( 7 - ( idxTo - idxFrom ) ) << ( idxFrom & 7 ) );
            }
        }
        else
        {
            // 分别搞一头一尾, 再 memset 中间
            auto idxFrom7 = idxFrom & 7;
            auto idxTo7 = idxTo & 7;
            if( v )
            {
                buf[ byteIdxFrom ] |= (byte)0xFFu << idxFrom7;
                buf[ byteIdxTo ] |= (byte)0xFFu >> ( 7 - idxTo7 );
            }
            else
            {
                buf[ byteIdxFrom ] &= ~( (byte)0xFFu << idxFrom7 );
                buf[ byteIdxTo ] &= ~( (byte)0xFFu >> ( 7 - idxTo7 ) );
            }
            if( idxFrom7 ) ++byteIdxFrom;
            if( idxTo7 ) --byteIdxTo;
            if( byteIdxFrom <= byteIdxTo )
            {
                memset( buf + byteIdxFrom, v ? 0xFFFFFFFFu : 0, byteIdxTo - byteIdxFrom );
            }
        }
    }

    void List<bool>::Resize( int capacity, bool init )
    {
        if( capacity == size ) return;
        if( capacity < size )
        {
            size = capacity;
            return;
        }
        Reserve( capacity );
        auto oldSize = size;
        size = capacity;
        if( init )
        {
            Fill( false, oldSize, capacity - 1 );
        }
    }

    void List<bool>::CleanUp() const
    {
        if( auto mod = size % 8 )
        {
            buf[ ( size - 1 ) / 8 ] &= ~( 0xFF << mod );
        }
    }






    void List<bool>::WriteTo( ByteBuffer& bb ) const
    {
        bb.VarWrite( (uint)size );
        if( size )
        {
            CleanUp();
            bb.Write( buf, ByteSize() );
        }
    }

    void List<bool>::FastWriteTo( ByteBuffer& bb ) const
    {
        bb.FastVarWrite( (uint)size );
        if( size )
        {
            CleanUp();
            bb.FastWrite( buf, ByteSize() );
        }
    }


    bool List<bool>::ReadFrom( ByteBuffer& bb )
    {
        int len;
        if( !bb.VarRead( *(uint*)&len ) || len < 0 ) return false;
        if( len == 0 )
        {
            Clear();
            return true;
        }
        auto byteLen = ( len - 1 ) / 8 + 1;
        if( bb.offset + byteLen > bb.dataLen ) return false;
        Reserve( len );
        size = len;
        memcpy( buf, bb.buf + bb.offset, byteLen );
        bb.offset += byteLen;

        return true;
    }

}




/*

#include "Lib/All.h"
#include <conio.h>
using namespace std;
using namespace xxx;


int main()
{
List<bool> bs;

auto dump = [&]
{
for( int i = 0; i < bs.Size(); ++i )
{
Cout( bs[ i ] ? "t " : "F " );
}
CoutLine();
};

bs.Push( false );
bs.Push( true );
bs.Resize( 9, false );  // 只扩容不初始化
dump();                 // 将输出 false true ....随机
bs.Fill( false, 2, 8 );
dump();                 // 输出 false true false 一串
bs.Fill( true, 8, 7 );
dump();                 // 最后 2 位应该输出 true
bs.Resize( 12, true );
dump();                 // 最后 3 位应该输出 false
bs.Resize( 16 );
bs.Fill( true, 12, 15 );
dump();                 // 最后 3 位应该输出 true
bs.Fill( true, 0, 0 );
dump();                 // 第 1 位应该输出 true
bs.Fill( false, 1, 0 );
dump();                 // 第 2 位应该输出 false
bs.Resize( 24 );
bs.FillFalse();
dump();                 // 全部输出 false
bs.FillFalse();
bs.Fill( true, 7, 7 );
dump();                 // 第 8 位输出 true
bs.FillFalse();
bs.Fill( true, 7, 8 );
dump();                 // 第 8, 9 位输出 true
bs.FillFalse();
bs.Fill( true, 0, 8 );
dump();                 // 第 1 ~ 9 位输出 true
bs.FillFalse();
bs.Fill( true, 8, 8 );
dump();                 // 第 9 位输出 true
bs.FillFalse();
bs.Fill( true, 23, 23 );
dump();                 // 第 24 位输出 true
bs.FillFalse();
bs.Fill( true, 15, 23 );
dump();                 // 第 16 ~ 24 位输出 true
bs.FillFalse();
bs.Fill( true, 16, 23 );
dump();                 // 第 17 ~ 24 位输出 true
bs.FillFalse();
bs.Fill( true, 8, 15 );
dump();                 // 第 9 ~ 16 位输出 true

bs.FillTrue();
bs.Fill( false, 7, 7 );
dump();                 // 第 8 位输出 false
bs.FillTrue();
bs.Fill( false, 7, 8 );
dump();                 // 第 8, 9 位输出 false
bs.FillTrue();
bs.Fill( false, 0, 8 );
dump();                 // 第 1 ~ 9 位输出 false
bs.FillTrue();
bs.Fill( false, 8, 8 );
dump();                 // 第 9 位输出 false
bs.FillTrue();
bs.Fill( false, 23, 23 );
dump();                 // 第 24 位输出 false
bs.FillTrue();
bs.Fill( false, 15, 23 );
dump();                 // 第 16 ~ 24 位输出 false
bs.FillTrue();
bs.Fill( false, 16, 23 );
dump();                 // 第 17 ~ 24 位输出 false
bs.FillTrue();
bs.Fill( false, 8, 15 );
dump();                 // 第 9 ~ 16 位输出 false


return 0;
}


*/