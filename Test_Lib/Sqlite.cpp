#include "Sqlite.h"

namespace Sqlite
{

    Connection::Connection()
        : _db( nullptr )
        , _writeable( false )
        , _existsQuery( nullptr )
    {
    }

    Connection::Connection( std::string const & fn, bool writeable )
        : _db( nullptr )
        , _fn( fn )
        , _writeable( writeable )
        , _existsQuery( nullptr )
    {
    }

    Connection::~Connection()
    {
        close();
    }

    void Connection::assign( std::string const & fn, bool writeable )
    {
        close();
        _fn = fn;
        _writeable = writeable;
    }

    bool Connection::open()
    {
        if( _db ) return false;
        int r = 0;
        if( _writeable )
        {
            r = sqlite3_open_v2( _fn.c_str(), &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr );
        }
        else
        {
            r = sqlite3_open_v2( _fn.c_str(), &_db, SQLITE_OPEN_READONLY, nullptr );
        }
        if( r != SQLITE_OK )
        {
            close();
            return false;
        }
        return true;
    }

    void Connection::close()
    {
        if( !_db ) return;
        clearQueries();
        sqlite3_close( _db );
        _db = nullptr;
    }


    Query * Connection::newQuery( std::string const & sql, int parameterCount /*= 0 */ )
    {
        Query * rtv = nullptr;
        if( !_db ) return rtv;
        rtv = new Query( this, sql, parameterCount );
        _qs.push_back( rtv );
        return rtv;
    }

    void Connection::deleteQuery( Query * q )
    {
        if( !q ) return;
        delete q;
        auto it = find( _qs.begin(), _qs.end(), q );
        if( it != _qs.end() )
        {
            _qs.erase( it );
        }
    }

    void Connection::clearQueries()
    {
        for( auto& q : _qs ) delete q;
        _qs.clear();
        _existsQuery = nullptr;
    }





    void Connection::beginTransaction()
    {
        execute( "BEGIN TRANSACTION" );
    }

    void Connection::commit()
    {
        execute( "COMMIT TRANSACTION" );
    }

    void Connection::rollback()
    {
        execute( "ROLLBACK TRANSACTION" );
    }

    void Connection::endTransaction()
    {
        execute( "END TRANSACTION" );
    }

    bool Connection::exists( std::string const & tn )
    {
        bool rtv = false;
        if( !_existsQuery )
        {
            _existsQuery = newQuery( "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?" );
        }
        ( *_existsQuery ).add( tn );
        _existsQuery->execute( [ &]( Sqlite::DataReader& dr )
        {
            rtv = dr.readInt32() > 0;
        } );
        return rtv;
    }

    bool Connection::execute( std::string const & sql )
    {
        if( !_db ) return false;
        return sqlite3_exec( _db, sql.c_str(), nullptr, nullptr, nullptr ) == SQLITE_OK;
    }




















    Query::Query()
        : _conn( nullptr )
        , _stmt( nullptr )
        , _vc( 0 )
        , _vi( 0 )
    {
    }

    Query::Query( Connection * conn, std::string const & sql, int parameterCount )
        : _conn( conn )
        , _stmt( nullptr )
        , _vi( 0 )
    {
        assign( sql, parameterCount );
    }

    Query::~Query()
    {
        if( _stmt ) sqlite3_finalize( _stmt );
    }

    bool Query::assign( std::string const & sql, int n )
    {
        _sql = sql;

        if( _stmt )
        {
            sqlite3_finalize( _stmt );
            _stmt = nullptr;
            _vi = 0;
        }

        if( sqlite3_prepare_v2( _conn->_db, sql.c_str(), (int)sql.size(), &_stmt, nullptr ) != SQLITE_OK )
        {
            if( _stmt )
            {
                sqlite3_finalize( _stmt );
                _stmt = nullptr;
            }
            return false;
        }

        if( n )
        {
            _vc = n;
        }
        else
        {
            // 数 sql 串里有多少个 ?, 当前先这样实现.
            _vc = 0;
            for( auto& c : _sql )
            {
                if( c == '?' ) ++_vc;
            }
        }

        return true;
    }

    bool Query::execute( RowReaderType rr )
    {
        if( !_stmt )
        {
            printf( "Query::execute failed. _stmt = nullptr" );
            return false;
        }
        if( _vc != _vi )
        {
            printf( "Query::execute failed. parameter count is not valid. _vc = %d, _vi = %d", _vc, _vi );
            return false;
        }
        _vi = 0;
        int ok = sqlite3_step( _stmt );
        if( !( ok == SQLITE_OK || ok == SQLITE_DONE || ok == SQLITE_ROW ) )
        {
            printf( "Query::execute sqlite3_step failed. code = %d, errmsg = %s", ok, sqlite3_errmsg( _conn->_db ) );
            return false;
        }
        if( rr )
        {
            DataReader dr( _stmt );
            while( ok == SQLITE_ROW )
            {
                dr.reset();
                rr( dr );
                ok = sqlite3_step( _stmt );
            }
        }
        ok = sqlite3_reset( _stmt );
        if( ok == SQLITE_OK || ok == SQLITE_DONE ) return true;
        printf( "Query::execute sqlite3_reset failed. code = %d", ok );
        return false;
    }

    bool Query::add( int v )
    {
        return sqlite3_bind_int( _stmt, ++_vi, v ) == SQLITE_OK;
    }

    bool Query::add( long long v )
    {
        return sqlite3_bind_int64( _stmt, ++_vi, v ) == SQLITE_OK;
    }

    bool Query::add( double v )
    {
        return sqlite3_bind_double( _stmt, ++_vi, v ) == SQLITE_OK;
    }

    bool Query::add( std::string const & v )
    {
        return sqlite3_bind_text( _stmt, ++_vi, v.c_str(), (int)v.size(), SQLITE_STATIC ) == SQLITE_OK;
    }

    bool Query::add( char const * s )
    {
        if( s ) return sqlite3_bind_text( _stmt, ++_vi, s, (int)strlen( s ), SQLITE_STATIC ) == SQLITE_OK;
        return sqlite3_bind_null( _stmt, ++_vi ) == SQLITE_OK;
    }

    bool Query::add( unsigned char const * buf, size_t len )
    {
        return sqlite3_bind_blob( _stmt, ++_vi, buf, (int)len, SQLITE_STATIC ) == SQLITE_OK;
    }









    DataReader::DataReader( sqlite3_stmt * stmt )
        : _stmt( stmt )
        , _index( 0 )
    {
    }

    bool DataReader::readAt( int columnIndex, char const * & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = (char const *)sqlite3_column_text( _stmt, columnIndex );
        return true;
    }

    bool DataReader::readAt( int columnIndex, std::string & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v.assign( (char const *)sqlite3_column_text( _stmt, columnIndex ) );
        return true;
    }

    bool DataReader::readAt( int columnIndex, int & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = sqlite3_column_int( _stmt, columnIndex );
        return true;
    }

    bool DataReader::readAt( int columnIndex, long long & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = sqlite3_column_int64( _stmt, columnIndex );
        return true;
    }

    bool DataReader::readAt( int columnIndex, double & v )
    {
        if( sqlite3_column_type( _stmt, columnIndex ) == SQLITE_NULL ) return false;
        v = sqlite3_column_double( _stmt, columnIndex );
        return true;
    }

    char const * DataReader::readCString()
    {
        char const * v = nullptr;
        readAt( _index++, v );
        return v;
    }

    std::string DataReader::readString()
    {
        std::string v;
        readAt( _index++, v );
        return v;
    }

    int DataReader::readInt32()
    {
        int v = 0;
        readAt( _index++, v );
        return v;
    }

    long long DataReader::readInt64()
    {
        long long v = 0;
        readAt( _index++, v );
        return v;
    }

    double DataReader::readDouble()
    {
        double v = 0;
        readAt( _index++, v );
        return v;
    }


    char const * DataReader::readCStringAt( int columnIndex )
    {
        char const * v = nullptr;
        readAt( columnIndex, v );
        return v;
    }

    std::string DataReader::readStringAt( int columnIndex )
    {
        std::string v;
        readAt( columnIndex, v );
        return v;
    }

    int DataReader::readInt32At( int columnIndex )
    {
        int v = 0;
        readAt( columnIndex, v );
        return v;
    }

    long long DataReader::readInt64At( int columnIndex )
    {
        long long v = 0;
        readAt( columnIndex, v );
        return v;
    }

    double DataReader::readDoubleAt( int columnIndex )
    {
        double v = 0;
        readAt( columnIndex, v );
        return v;
    }



    bool DataReader::read( char const * & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( std::string & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( int & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( long long & v )
    {
        return readAt( _index++, v );
    }

    bool DataReader::read( double & v )
    {
        return readAt( _index++, v );
    }

    void DataReader::reset()
    {
        _index = 0;
    }



    
}
