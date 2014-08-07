#include "Lib/All.h"
#include "PacketTestPackets.h"

namespace PacketTestPackets
{
    ushort Foo::getTypeID()
    {
        return 0;
    }
    void Foo::fill( FlatBuffer& fb
        , int( &pIsss )[3][2][1] )
    {
        fb.writes(
            pIsss );
    }

    void Foo::writeBuffer( FlatBuffer& fb ) const
    {
        fb.writes(
            _isss );
    }

    bool Foo::readBuffer( FlatBuffer& fb )
    {
        return fb.reads(
            _isss );
    }

    void Foo::writeBufferDirect( FlatBuffer& fb ) const
    {
        fb.writesDirect(
            _isss );
    }

    int Foo::getWriteBufferSize() const
    {
        return 
            BufferUtils::getSizes( _isss );
    }

}
