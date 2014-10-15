#include "Lib/All.h"
using namespace std;
using namespace xxx;


struct Bar : public CorBase<Bar>
{
    String txt;
    void Init( String _txt )
    {
        txt = _txt;
    }
    bool Process( int _ticks ) override
    {
        COR_BEGIN;
        Cout( txt, " say: i'm living..." );
        COR_SLEEP( 5 );
        Cout( txt, " say: i'm dieing..." );
        COR_END;
    }
};

struct Foo : public CorBase<Foo>
{
    REF_DECL( Bar, bar );

    void Init( Bar* _bar )
    {
        REF_SET( bar, _bar );
    }
    void EnsureRefs() override
    {
        REF_ENSURE( bar );
    }

    int i;
    bool Process( int _ticks ) override
    {
        EnsureRefs();
        COR_BEGIN;
        for( i = 0; i < 9; ++i )
        {
            Cout( "Foo's i = ", i );
            if( bar )
            {
                Cout( "Foo's Bar alive!" );
            }
            COR_YIELD;
        }
        COR_END;
    }
};

int main()
{
    CorManager<Cor> fbm;
    fbm.CreateItem<Foo>( fbm.CreateItem<Bar>( "Bar" ) );
    while( fbm.Process() );

    system( "pause" );
    return 0;
}
