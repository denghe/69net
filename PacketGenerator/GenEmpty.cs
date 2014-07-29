using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    public class GenEmpty
    {
        public static string Gen( Template sd, string outputDirPath, string fn ) {

            var sb = new StringBuilder();





            try {
                sb.WriteToFile( Path.Combine( outputDirPath, fn + "_packets.lua" ), false );
            }
            catch( Exception ex ) {
                return ex.Message;
            }

            return "";
        }

    }
}
