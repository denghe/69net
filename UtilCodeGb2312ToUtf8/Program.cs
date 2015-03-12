using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace UtilCodeGb2312ToUtf8
{
    static class Program
    {
        static bool Bit( this byte a, int idx )
        {
            return ( ( 1 << ( idx - 1 ) ) & a ) > 0;
        }
        static bool IsUtf8( byte[] bytes )
        {
            // 需要用某种方式来猜一下编码。
            // 以 utf8 文件为例，当扫到高位 bit 为 1 的 byte 时，
            // 它应该是类似 10 110 1110 ... 这样的长相，
            // 并且接下来有 0 前面 1 的个数字节，是以 10 开头的。
            // 满足这个规律，我们就可以认为它是个 utf8 串了
            for( int i = 0; i < bytes.Length; ++i )
            {
                var c = bytes[ i ];
                if( c > 0x7Fu )
                {
                    int len = 0;
                    if( c.Bit( 8 ) ) len++; // 10xxxxxx
                    if( c.Bit( 7 ) ) len++; // 110xxxxx
                    else goto Calc;
                    if( c.Bit( 6 ) ) len++; // 1110xxxx
                    else goto Calc;
                    if( c.Bit( 5 ) ) len++; // 11110xxx
                    else goto Calc;
                    if( c.Bit( 4 ) ) len++; // 111110xx
                    else goto Calc;
                    if( c.Bit( 3 ) ) len++; // 1111110x
                    if( c.Bit( 2 ) ) return false;
                Calc:
                    if( i >= bytes.Length - len ) return false;
                    for( int j = i + 1; j < i + len; j++ )
                    {
                        c = bytes[ j ];
                        if( !( c.Bit( 8 ) && !c.Bit( 7 ) ) ) return false;
                    }
                    return true;
                }
            }
            return true;
        }

        static void Main( string[] args )
        {
            int n = 0;
            var dir = System.Environment.CurrentDirectory;
            var fns = Directory.GetFiles( dir, "*.*", SearchOption.AllDirectories );
            var gb2312encoding = Encoding.GetEncoding( "gb2312" );
            foreach( var fn in fns )
            {
                var extName = new FileInfo( fn ).Extension.ToLower();
                if( extName == ".cpp"
                    || extName == ".h"
                    || extName == ".hpp"
                    || extName == ".inc" )
                {
                    var bytes = File.ReadAllBytes( fn );
                    if( !IsUtf8( bytes ) )
                    {
                        Console.WriteLine( "convert to utf8..." + fn );
                        ++n;
                        var txt = gb2312encoding.GetString( bytes );
                        bytes = Encoding.UTF8.GetBytes( txt );
                        File.WriteAllBytes( fn, bytes );
                    }
                }
            }
            if( n > 0 )
            {
                Console.WriteLine( "count: " + n );
            }
            else
            {
                Console.WriteLine( "no gb2312 code file found." );
            }

            Console.WriteLine( "press any to continue..." );
            Console.Read();
        }
    }
}
