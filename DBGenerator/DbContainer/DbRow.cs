using System;
using System.Collections.Generic;

namespace DBContainer
{

    public partial class DbRow
    {
        public DbTable          parent          = null;
        public int              rowIndex        = 0;

        public DbValue this[ int colIdx ]
        {
            get
            {
                return new DbValue { parent = this, columnIndex = colIdx };
            }
        }
        public DbValue this[ string colName ]
        {
            get
            {
                return new DbValue { parent = this, columnIndex = parent.getIndexByColumnName( colName ) };
            }
        }

    }

}
