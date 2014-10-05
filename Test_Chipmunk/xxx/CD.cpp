#include "Precompile.h"

namespace xxx
{

    CdGrid::~CdGrid()
    {
        for( int i = 0; i < items.size(); ++i )
        {
            delete items[ i ]->key;
        }
        for( int i = 0; i < freeItems.size(); ++i )
        {
            delete freeItems[ i ];
        }
    }

    void CdGrid::Reserve( int capacity )
    {
        for( int i = 0; i < capacity; ++i )
        {
            freeItems.push( new CdItem() );
        }
    }

    void CdGrid::Init( CdSize _gridDiameter, int _rowCount, int _columnCount )
    {
        Clear();
        gridDiameter = _gridDiameter;
        rowCount = _rowCount;
        columnCount = _columnCount;
        cellDiameter = { _gridDiameter.w / _columnCount, _gridDiameter.h / _rowCount };
        cellRadius = { cellDiameter.w / 2, cellDiameter.h / 2 };
        cells.resize( _rowCount * _columnCount );
        for( int ri = 0; ri < _rowCount; ++ri )
        {
            for( int ci = 0; ci < _columnCount; ++ci )
            {
                auto &c = cells[ ri*_columnCount + ci ];
                c.index = { ci, ri };
            }
        }
    }

    CdItem* CdGrid::CreateItem()
    {
        if( freeItems.size() )
        {
            return freeItems.top_pop();
        }
        return new CdItem();
    }

    void CdGrid::DestroyItem( CdItem* _item )
    {
        assert( items.find( _item ) );
        auto& cs = _item->cells;
        for( int i = 0; i < cs.size(); ++i )
        {
            cs[ i ]->items.erase( _item );
        }
        cs.clear();
        items.erase( _item );
        freeItems.push( _item );
    }

    void CdGrid::Clear()
    {
        for( int i = 0; i < items.size(); ++i )
        {
            auto& p = items[ i ]->key;
            p->cells.clear();
            freeItems.push( p );
        }
        items.clear();
        for( int i = 0; i < cells.size(); ++i )
        {
            cells[ i ].items.clear();
        }
    }

    void CdGrid::Reindex( CdItem* _item )
    {
        auto& cs = _item->cells;
        for( int i = 0; i < cs.size(); ++i )
        {
            cs[ i ]->items.erase( _item );
        }
        cs.clear();

        CdPoint bottomLeft = { _item->pos.x - _item->radius.w, _item->pos.y - _item->radius.h };
        int ci1 = bottomLeft.x / cellDiameter.w;
        int ri1 = bottomLeft.y / cellDiameter.h;
        assert( ci1 >= 0 && ci1 < columnCount && ri1 >= 0 && ri1 < rowCount );

        CdPoint topRight = { _item->pos.x + _item->radius.w, _item->pos.y + _item->radius.h };
        int ci2 = bottomLeft.x / cellDiameter.w;
        int ri2 = bottomLeft.y / cellDiameter.h;
        assert( ci2 >= 0 && ci2 < columnCount && ri2 >= 0 && ri2 < rowCount );

        for( int ri = ri1; ri <= ri2; ++ri )
        {
            for( int ci = ci1; ci <= ci2; ++ci )
            {
                cs.push( &cells[ ri * columnCount + ci ] );
            }
        }
    }

    void CdGrid::Reindex( CdItem* _item, CdPoint const& _pos )
    {
        CdPoint bottomLeft = { _pos.x - _item->radius.w, _pos.y - _item->radius.h };
        int ci1 = bottomLeft.x / cellDiameter.w;
        int ri1 = bottomLeft.y / cellDiameter.h;
        assert( ci1 >= 0 && ci1 < columnCount && ri1 >= 0 && ri1 < rowCount );

        CdPoint topRight = { _pos.x + _item->radius.w, _pos.y + _item->radius.h };
        int ci2 = bottomLeft.x / cellDiameter.w;
        int ri2 = bottomLeft.y / cellDiameter.h;
        assert( ci2 >= 0 && ci2 < columnCount && ri2 >= 0 && ri2 < rowCount );

        auto& cs = _item->cells;
        for( int i = cs.size() + 1; i >= 0; --i )
        {
            auto& idx = cs[ i ]->index;
            if( idx.y < ri1 || idx.y > ri2 || idx.x < ci1 || idx.x > ci2 )
            {
                cs[ i ]->items.erase( _item );
                cs.erase( i );
            }
        }

        for( int ri = ri1; ri <= ri2; ++ri )
        {
            for( int ci = ci1; ci <= ci2; ++ci )
            {
                auto c = &cells[ ri * columnCount + ci ];
                if( cs.find( c ) < 0 )
                {
                    cs.push( c );
                    c->items.insert( _item );
                }
            }
        }
    }

    int CdGrid::GetNearItems( List<CdItem*> container, CdItem* _item )
    {
        ++autoFlag;
        if( autoFlag == 0 )
        {
            autoFlag = 1;
            ResetAllFlag();
        }
        container.clear();
        auto& cs = _item->cells;
        for( int i = 0; i < cs.size(); ++i )
        {
            auto& cis = cs[ i ]->items;
            for( int j = 0; j < cis.size(); ++j )
            {
                auto& item = cis[ j ]->key;
                if( item->flag == autoFlag ) continue;
                item->flag = autoFlag;
                container.push( item );
            }
        }
        return container.size();
    }


    int CdGrid::GetCollisionItems( List<CdItem*> container, CdItem* _item )
    {
        ++autoFlag;
        if( autoFlag == 0 )
        {
            autoFlag = 1;
            ResetAllFlag();
        }
        container.clear();
        auto& cs = _item->cells;
        for( int i = 0; i < cs.size(); ++i )
        {
            auto& cis = cs[ i ]->items;
            for( int j = 0; j < cis.size(); ++j )
            {
                auto& item = cis[ j ]->key;
                if( item->flag == autoFlag ) continue;

                if( ( _item->groupId & item->groupId ) != 0 )
                {
                    if( _item->groupId == item->groupId ) continue;
                }
                if( ( _item->layerMask & item->layerMask ) == 0 ) continue;

                uint r;
                r = _item->radius.w + item->radius.w;
                if( (uint)( _item->pos.x - item->pos.x + r ) > r + r ) continue;
                r = _item->radius.h + item->radius.h;
                if( (uint)( _item->pos.y - item->pos.y + r ) > r + r ) continue;

                item->flag = autoFlag;
                container.push( item );
            }
        }
        return container.size();
    }

    void CdGrid::ResetAllFlag()
    {
        for( int i = 0; i < items.size(); ++i )
        {
            items[ i ]->key->flag = 0;
        }
        for( int i = 0; i < freeItems.size(); ++i )
        {
            freeItems[ i ]->flag = 0;
        }
    }

}
