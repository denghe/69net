UInt32 i = 0;
while (i < len) {
    var c = s[i++];
    if (c == '%') {
        c = s[i++];
        switch(c) {
        case '%': { Append('%'); break; }
#include "StringAFC.inc"
        default:
            assert ( false );
            break;
        }
    } else
        Append(c);
}
