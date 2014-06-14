#ifndef __SQLITE___H__
#define __SQLITE___H__

#include "sqlite3.h"
#include <string>
#include <vector>
#include <functional>

namespace Sqlite
{
    struct Query;

    struct Connection
    {
        // zero vars
        Connection();

        // call assign
        Connection( std::string const & fn, bool writeable = true );

        // call close
        virtual ~Connection();

        // close & set db file full path
        void assign( std::string const & fn, bool writeable = true );

        // open db, success return true
        bool open();

        // clear stmts, close db
        void close();

        // create stmt. n : num of parameters
        Query * newQuery( std::string const & sql, int n = 0 );

        // delete stmt
        void deleteQuery( Query * q );

        // clear stmts
        virtual void clearQueries();

        // execute BEGIN TRANSACTION
        void beginTranslation();

        // execute COMMIT TRANSACTION( same as end )
        void commit();

        // execute END TRANSACTION( same as commit )
        void endTranslation();

        // execute ROLLBACK TRANSACTION
        void rollback();

        // execute SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = ?
        bool exists( std::string const & tn );

        // simple exec
        bool execute( std::string const & sql );


        friend Query;
    protected:
        sqlite3 *           _db;
        std::string              _fn;
        bool                _writeable;
        std::vector<Query*>      _qs;
        Query*              _existsQuery;

        // = delete
        Connection( Connection const & other );
        Connection & operator=( Connection const & other );
    };

    class DataReader
    {
    public:
        DataReader( sqlite3_stmt * stmt );

        // fill mode, if value is null, return false
        bool readAt( int columnIndex, char const * & v );
        bool readAt( int columnIndex, std::string & v );
        bool readAt( int columnIndex, int & v );
        bool readAt( int columnIndex, long long & v );
        bool readAt( int columnIndex, double & v );

        // return mode
        char const * readCStringAt( int columnIndex );
        std::string readStringAt( int columnIndex );
        int readInt32At( int columnIndex );
        long long readInt64At( int columnIndex );
        double readDoubleAt( int columnIndex );

        // _index++
        bool read( char const * & v );
        bool read( std::string & v );
        bool read( int & v );
        bool read( long long & v );
        bool read( double & v );

        char const * readCString();
        std::string readString();
        int readInt32();
        long long readInt64();
        double readDouble();

    private:
        sqlite3_stmt * _stmt;
        int _index;
    };

    struct Query
    {
        typedef std::function<void( DataReader& dr )> RowReaderType;

        /*
        execute sql, read output data. only support single sql segment
        sample:

        auto q = conn.newQuery( "REPLACE INTO test1 ( `name`, `desc` ) VALUES ( ?, ? )" );
        (*q) << ... << ...;
        q->execute();

        Foo o;
        q->assign( "SELECT * from test1" );
        q->execute( []( Sqlite::DataReader& dr )
        {
            dr >> o.xxx >> o.eee >> o.fff ...
        } );
        */
        bool execute( RowReaderType rr = nullptr );

        // reset sql query
        bool assign( std::string const & sql, int n = 0 );

        // push parameter values
        //

        bool add( int v );
        bool add( long long v );
        bool add( double v );
        bool add( std::string const & v );
        bool add( char const * v );                 // nullptr = NULL
        bool add( char const * buf, size_t len );
        template<typename T>
        void add( T const & v )
        {
            static_assert( std::is_enum<T>::value, "must be enum type" );
            if( sizeof( T ) <= 4 ) add( (int)v );
            else add( (long long)v );
        }
        template<typename T>
        Query & operator << ( T const & v ) { add( v ); return *this; }


        friend Connection;
    private:
        Query();
        Query( Connection * conn, std::string const & sql, int n = 0 );
        ~Query();

        Connection *        _conn;
        sqlite3_stmt *      _stmt;
        std::string         _sql;
        int                 _vc;    // value count
        int                 _vi;    // value index

        // = delete
        Query( Query const & other );
        Query & operator=( Query const & other );
    };

}

#endif

