using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DBGenerator
{
    class Program
    {
        static void Main( string[] args )
        {
            var info = MyInfo.GetInfo();

            /*
             
            players
            int  str
            id  name
            1   "asdf"
            2   "qwer"
             
            chars
            int         int         ....
            id        playerId     ...
            1         1           
            2         1           
            3         1           

            vector<DbPlayers> allPlayers
            class DbPlayers
            {
             * id
             * name
            }
            class DbChars
             * 
             * 
             * 
            class Player
            {
             * id
             * name
             vector<Char*> chars;
            }
            class Char
            {
            }
            * 
            * 
            * 
            * 
            */
        }
    }
}
