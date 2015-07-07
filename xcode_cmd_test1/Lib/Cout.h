#ifndef _COUT__H_
#define _COUT__H_

namespace xxx
{

    //// 带 Dump( String& ) const 函数的都可以 被正常 Dump
    HAS_FUNC( HasFunc_Dump, Dump, void ( T::* )( String& ) const );
    template<typename T>
    typename std::enable_if<HasFunc_Dump<T>::value, void>::type DumpCoreSwitch( String& s, T const& v )
    {
        v.Dump( s );
    };
    template<typename T>
    typename std::enable_if<!HasFunc_Dump<T>::value, void>::type DumpCoreSwitch( String& s, T const& v )
    {
        s.Append( v );
    };
    template<typename T>
    void DumpCore( String& s, T const& v )
    {
        DumpCoreSwitch( s, v );
    }

    template<typename T>
    void Dump( T const& v )
    {
        String s;
        DumpCore( s, v );
        s.Append( '\n' );
        fwrite( s.C_str(), 1, s.Size(), stdout );
    }

    template<typename ...TS>
    void Cout( TS const& ...parms );

    template<typename ...TS>
    void Coutn( TS const& ...parms );

    template<typename ...TS>
    void CoutPos( short x, short y, TS const& ...parms );

}

#endif
