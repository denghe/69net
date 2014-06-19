var format = (T *)s;
while (var c = *format++) {
    if (c == '%') {
        c = *format++;
        switch(c) {
        case '%': { WriteChar(buf, offset, c); break; }
#include "StringsWFC.inc"
        default: {
            assert ( false );
            break; }
        }
    } else
        WriteChar(buf, offset, c);
}
