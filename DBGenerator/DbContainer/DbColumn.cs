using System;
using System.Collections.Generic;
using System.Data;

namespace DBContainer
{
    public partial class DbColumn
    {
        public DbTable          parent          = null;
        public int              columnIndex     = 0;
        public string           name            = "";
        public bool             nullable        = false;
        public DbDataTypes      dataType        = DbDataTypes.Unknown;
        public bool             autoIncrement   = false;
        public bool             primaryKey      = false;
        public bool             timestamp       = false;
        public string           comment         = "";
        public bool isReadonly
        {
            get { return autoIncrement | timestamp; }
        }
    }

}
