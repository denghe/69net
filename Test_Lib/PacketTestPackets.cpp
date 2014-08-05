#include "Lib/All.h"
#include "PacketTestPackets.h"

namespace PacketTestPackets
{
    ushort Foo::getTypeID()
    {
        return 0;
    }
    void Foo::fill( FlatBuffer& fb
        , int( &pIsss )[3][2][1]
        , Dict<String,List<byte[8]>>( &pEee )[5][3] )
    {
        fb.writes(
            pIsss, 
            pEee );
    }

    void Foo::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _isss, 
            _eee );
    }

    bool Foo::readBuffer( FlatBuffer& fb )
    {
        return fb.reads(
            _isss, 
            _eee );
    }

    void Foo::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect(
            _isss, 
            _eee );
    }

    int Foo::getWriteBufferSize() const
    {
        return 
            BufferUtils::getSizes( _isss ) + 
            BufferUtils::getSizes( _eee );
    }

}
