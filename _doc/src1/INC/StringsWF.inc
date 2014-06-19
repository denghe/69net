Char C;
Byte c1, c2, c3;
UInt32 i = 0, mode = 0;         // mode 为 1 时为取参数索引&转义态. 为 0 时直接输出字符
do {
	c1 = (Byte)s[i++];
    if (c1 == 0)
        break;
	if (c1 < 0x80u) C = c1;
	else if ((c1 & 0xE0u) == 0xE0u)
	{
        c2 = (Byte)s[i++];
		if (c2) {
            c3 = (Byte)s[i++];
			if (c3)
				C = ((c1 & 0x0Fu) << 12) | ((c2 & 0x3Fu) << 6) | (c3 & 0x3Fu);
			else
                break;
        }
		else
            break;
	}
	else if ((c1 & 0xC0u) == 0xC0u) {
        c2 = (Byte)s[i++];
		if (c2)
			C = ((c1 & 0x1Fu) << 6) | (c2 & 0x3Fu);
		else
            break;
	}
    if (mode == 0) {
        if (C == '%')
            mode = 1;
        else
            WriteChar(buf, offset, C);
    }
    else {
        switch(C) {
        case '%': { WriteChar(buf, offset, C); break; }
#include "StringsWFC.inc"
        default:
            assert ( false );
            break;
        }
        mode = 0;
    }
} while (i < len);
