using System;
using System.Collections.Generic;
using System.Data;

namespace DBContainer
{
    public class DbQueryResult
    {
        public int              affectedRows    = -1;
        public List<DbTable>    tables          = new List<DbTable>();
        public string           schema          = "";
    }
}
