UInt32 i = 0;
while (i < len) {
    var c = s[i++];
    if (c == '%') {
        c = s[i++];
        switch(c) {
        case '%': { WriteChar('%'); break; }
#include "ConsoleWFC.inc"
        default:
            assert ( false );
            break;
        }
    } else
        WriteChar(c);
}
