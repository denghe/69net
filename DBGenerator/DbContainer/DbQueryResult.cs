using System;
using System.Collections.Generic;
using System.Data;

namespace DBContainer
{
    public class DbQueryResult
    {
        public int              _affectedRows   = -1;
        public List<DbTable>    _tables         = new List<DbTable>();
        public string           _schema         = "";
    }
}
