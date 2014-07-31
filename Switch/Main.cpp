// 头文件引用区
#include "Lib/All.h"


#define PACKET_CLASS_HEADER( TN )           \
public:                                     \
TN();                                       \
~TN();                                      \
TN( TN const& other );                      \
TN( TN&& other );                           \
TN& operator=( TN const& other );           \
TN& operator=( TN&& other );                \
void writeBuffer( FlatBuffer& fb ) const;   \
bool readBuffer( FlatBuffer& fb );          \
String toString() const;                    \
void toString( String& s ) const;           \
static ushort getTypeID();


// 生成所有前置声明: 含 namespace
namespace SwitchPackets             // 根命名空间: 模板名 + Packets
{
    class Ping;
    // ... other class
}
// ... other namespace


namespace SwitchPackets
{
    class Ping : Memmoveable
    {
        PACKET_CLASS_HEADER( Ping );

        static void fillBuffer( FlatBuffer& fb );   // todo: parameters
        // todo: getXxxx setXxxx funcs
    };

    ushort Ping::getTypeID()
    {
        return 0;
    }

    void Ping::writeBuffer( FlatBuffer& fb ) const
    {

    }

    bool Ping::readBuffer( FlatBuffer& fb )
    {
        return true;
    }
}


int main()
{

    system( "pause" );
    return 0;
}

