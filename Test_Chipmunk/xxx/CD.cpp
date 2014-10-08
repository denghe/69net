#include "Precompile.h"

namespace xxx
{
    CdGrid::CdGrid()
    {
        cellRadius = { 0, 0 };
        cellDiameter = { 0, 0 };
        gridDiameter = { 0, 0 };
        rowCount = 0;
        columnCount = 0;
        autoFlag = 0;
    }

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

    void CdGrid::Reserve( int _capacity )
    {
        for( int i = 0; i < _capacity; ++i )
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
                c.ci = ci;
                c.ri = ri;
            }
        }
    }

    CdItem* CdGrid::CreateItem()
    {
        CdItem* rtv;
        if( freeItems.size() )
        {
            rtv = freeItems.top_pop();
            assert( rtv->cells.size() == 0 );
        }
        else
        {
            rtv = new CdItem();
        }
        rtv->radius = { 0, 0 };
        rtv->pos = { 0, 0 };
        rtv->groupId = 0;
        rtv->layerMask = 0xFFFFFFFF;
        rtv->flag = 0;
        rtv->ci1 = rtv->ci2 = rtv->ri1 = rtv->ri2 = 0;
        rtv->userData = nullptr;
        items.insert( rtv );
        return rtv;
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

    void CdGrid::Index( CdItem* _item )
    {
        auto& cs = _item->cells;
        for( int i = 0; i < cs.size(); ++i )
        {
            cs[ i ]->items.erase( _item );
        }
        cs.clear();

        CdPoint p = { _item->pos.x - _item->radius.w, _item->pos.y - _item->radius.h };
        int ci1 = p.x / cellDiameter.w;
        int ri1 = p.y / cellDiameter.h;
        assert( ci1 >= 0 && ci1 < columnCount && ri1 >= 0 && ri1 < rowCount );

        p = { _item->pos.x + _item->radius.w, _item->pos.y + _item->radius.h };
        int ci2 = p.x / cellDiameter.w;
        int ri2 = p.y / cellDiameter.h;
        assert( ci2 >= 0 && ci2 < columnCount && ri2 >= 0 && ri2 < rowCount );

        for( int ri = ri1; ri <= ri2; ++ri )
        {
            for( int ci = ci1; ci <= ci2; ++ci )
            {
                auto c = &cells[ ri * columnCount + ci ];
                cs.push( c );
                c->items.insert( _item );
            }
        }

        _item->ci1 = ci1;
        _item->ci2 = ci2;
        _item->ri1 = ri1;
        _item->ri2 = ri2;
    }

    void CdGrid::Update( CdItem* _item, CdPoint const& _pos )
    {
        _item->pos = _pos;

        CdPoint p = { _pos.x - _item->radius.w, _pos.y - _item->radius.h };
        int ci1 = p.x / cellDiameter.w;
        int ri1 = p.y / cellDiameter.h;
        assert( ci1 >= 0 && ci1 < columnCount && ri1 >= 0 && ri1 < rowCount );

        p = { _pos.x + _item->radius.w, _pos.y + _item->radius.h };
        int ci2 = p.x / cellDiameter.w;
        int ri2 = p.y / cellDiameter.h;
        assert( ci2 >= 0 && ci2 < columnCount && ri2 >= 0 && ri2 < rowCount );

        if( _item->ci1 == ci1
            && _item->ci2 == ci2
            && _item->ri1 == ri1
            && _item->ri2 == ri2 ) return;

        _item->ci1 = ci1;
        _item->ci2 = ci2;
        _item->ri1 = ri1;
        _item->ri2 = ri2;

        auto& cs = _item->cells;
        for( int i = cs.size() - 1; i >= 0; --i )
        {
            auto& c = *cs[ i ];
            if( c.ri < ri1 || c.ri > ri2 || c.ci < ci1 || c.ci > ci2 )
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
                if( cs.find( c ) >= 0 ) continue;
                cs.push( c );
                c->items.insert( _item );
            }
        }
    }


    void CdGrid::IncreaseFlag()
    {
        ++autoFlag;
        if( autoFlag ) return;
        autoFlag = 1;
        for( int i = 0; i < items.size(); ++i )
        {
            items[ i ]->key->flag = 0;
        }
        for( int i = 0; i < freeItems.size(); ++i )
        {
            freeItems[ i ]->flag = 0;
        }
    }


    int CdGrid::GetNearItems( List<CdItem*>& _container, CdItem* _item )
    {
        IncreaseFlag();
        _container.clear();
        auto& cs = _item->cells;
        for( int i = 0; i < cs.size(); ++i )
        {
            auto& cis = cs[ i ]->items;
            for( int j = 0; j < cis.size(); ++j )
            {
                auto& item = cis[ j ]->key;
                if( item->flag == autoFlag ) continue;
                item->flag = autoFlag;
                _container.push( item );
            }
        }
        return _container.size();
    }

    bool CdGrid::CheckCollision( CdItem* _a, CdItem* _b )
    {
        uint r;
        r = _a->radius.w + _b->radius.w;
        if( (uint)( _a->pos.x - _b->pos.x + r ) > r + r ) return false;
        r = _a->radius.h + _b->radius.h;
        if( (uint)( _a->pos.y - _b->pos.y + r ) > r + r ) return false;
        return true;
    }

    bool CdGrid::CheckCollision( CdItem* _a, CdPoint _pos )
    {
        return _a->pos.x - _a->radius.w <= _pos.x
            && _a->pos.x + _a->radius.w >= _pos.x
            && _a->pos.y - _a->radius.h <= _pos.y
            && _a->pos.y + _a->radius.h >= _pos.y;
    }

    int CdGrid::GetCollisionItems( List<CdItem*>& _container, CdItem* _item )
    {
        IncreaseFlag();
        _container.clear();
        _item->flag = autoFlag;     // 防止目标 item 被加入集合
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

                if( !CheckCollision( _item, item ) ) continue;

                item->flag = autoFlag;
                _container.push( item );
            }
        }
        return _container.size();
    }

    // 从上面的函数精简而来
    CdItem* CdGrid::GetCollisionItem( CdItem* _item )
    {
        auto& cs = _item->cells;
        for( int i = 0; i < cs.size(); ++i )
        {
            auto& cis = cs[ i ]->items;
            for( int j = 0; j < cis.size(); ++j )
            {
                auto& item = cis[ j ]->key;
                if( item == _item ) continue;

                if( ( _item->groupId & item->groupId ) != 0 )
                {
                    if( _item->groupId == item->groupId ) continue;
                }
                if( ( _item->layerMask & item->layerMask ) == 0 ) continue;

                if( !CheckCollision( _item, item ) ) continue;

                return item;
            }
        }
        return nullptr;
    }

    int CdGrid::GetItems( List<CdItem*>& _container, CdPoint const& _pos )
    {
        _container.clear();
        int ci = _pos.x / cellDiameter.w;
        int ri = _pos.y / cellDiameter.h;
        auto c = &cells[ ri * columnCount + ci ];
        for( int i = 0; i < c->items.size(); ++i )
        {
            auto& o = c->items[ i ]->key;
            if( CheckCollision( o, _pos ) )
            {
                _container.push( o );
            }
        }
        return _container.size();
    }

    CdItem* CdGrid::GetItem( CdPoint const& _pos )
    {
        int ci = _pos.x / cellDiameter.w;
        int ri = _pos.y / cellDiameter.h;
        auto c = &cells[ ri * columnCount + ci ];
        for( int i = 0; i < c->items.size(); ++i )
        {
            auto& o = c->items[ i ]->key;
            if( CheckCollision( o, _pos ) )
            {
                return o;
            }
        }
        return nullptr;
    }

}
