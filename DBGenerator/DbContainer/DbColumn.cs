using System;
using System.Collections.Generic;
using System.Data;

namespace DBContainer
{
    public partial class DbColumn
    {
        public DbTable          _parent         = null;
        public int              _columnIndex    = 0;
        public string           _name           = "";
        public bool             _nullable       = false;
        public DbDataTypes      _dataType       = DbDataTypes.Unknown;
        public bool             _autoIncrement  = false;
        public bool             _primaryKey     = false;
        public bool             _timestamp      = false;
        public string           _comment        = "";
        public bool _readonly
        {
            get { return _autoIncrement | _timestamp; }
        }
    }

}
