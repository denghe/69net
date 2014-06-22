#include <stdint.h>
#include <stdlib.h>  
#include <stdarg.h>  
  
#ifndef INCLUDED  
  
#define PUSH_CHAR(x) do { if (ret < maxlen) { *p++ = x; ret++; } } while (0)  
#define FRACTION_SIZE 23  
#define EXP_SIZE 8  
#define FTOA_INT int32_t  
  
typedef union {  
    int32_t L;  
    float F;  
} LF_t;  
  
int  
ftoa(float f, char *outbuf, int maxlen, int precision)   
#endif // INCLUDED  
{  
    FTOA_INT mantissa, int_part, int_part2, frac_part;  
    int16_t exp2;  
    int sign, i, m, max;  
    LF_t x;  
    char c, *p = outbuf;  
    int ret = 0;  
  
    x.F = f;  
  
    //exp2 = (unsigned char)(x.L >> 23);  
    exp2 = (((((FTOA_INT)1) << EXP_SIZE) - 1) & (x.L >> FRACTION_SIZE)); /* JEB fixed for 16-bit char F2xxx */   
    mantissa = (x.L & ((((FTOA_INT)1) << FRACTION_SIZE) - 1));  
    //printf("%x\n", x.L);  
    if (x.L >= 0) sign = +1; else sign = -1;  
    if (exp2 == ((((FTOA_INT)1) << EXP_SIZE) - 1)) {  
        if (mantissa == 0) {  
            if (sign == -1)  
                PUSH_CHAR('-');  
            PUSH_CHAR('i');  
            PUSH_CHAR('n');  
            PUSH_CHAR('f');  
            return ret;  
        } else {  
            PUSH_CHAR('n');  
            PUSH_CHAR('a');  
            PUSH_CHAR('n');  
            return ret;  
        }  
    }  
  
    exp2 -= ((((FTOA_INT)1) << EXP_SIZE) >> 1) - 1; //127;  
    mantissa |= (((FTOA_INT)1) << FRACTION_SIZE);  
    frac_part = 0;  
    int_part = 0;  
  
    if (exp2 >= FRACTION_SIZE)  
        int_part = mantissa << (exp2 - FRACTION_SIZE);  
    else if (exp2 >= 0) {  
        int_part = mantissa >> (FRACTION_SIZE - exp2);  
        frac_part = (mantissa << (exp2 + 1)) & ((((FTOA_INT)1) << (FRACTION_SIZE + 1)) - 1); //0xFFFFFF;  
    }  
    else /* if (exp2 < 0) */  
        frac_part = (mantissa & ((((FTOA_INT)1) << (FRACTION_SIZE + 1)) - 1)) >> -(exp2 + 1);  
  
    if (int_part == 0) {  
        if (sign == -1)  
            PUSH_CHAR('-');  
        PUSH_CHAR('0');  
    } else {  
        while (int_part != 0) {  
            int_part2 = int_part / 10;  
            PUSH_CHAR((char)(int_part - ((int_part2 << 3) + (int_part2 << 1)) + '0'));  
            int_part = int_part2;  
        }  
        if (sign == -1)  
            PUSH_CHAR('-');  
        // Reverse string  
        for (i = 0; i < ret / 2; i++) {  
            c = outbuf[i];  
            outbuf[i] = outbuf[ret - i - 1];  
            outbuf[ret - i - 1] = c;  
        }  
    }  
    if (precision != 0)  
        PUSH_CHAR('.');  
  
    max = maxlen - (int)(p - outbuf);  
    if (max > precision)  
        max = precision;  
    /* print BCD */  
    for (m = 0; m < max; m++) {  
        /* frac_part *= 10; */  
        frac_part = (frac_part << 3) + (frac_part << 1);  
  
        PUSH_CHAR((char)((frac_part >> (FRACTION_SIZE + 1)) + '0'));  
        frac_part &= ((((FTOA_INT)1) << (FRACTION_SIZE + 1)) - 1); //0xFFFFFF;  
    }  
    /* delete ending zeroes */  
    //for (--p; p[0] == '0' && p[-1] != '.'; --p, --ret) ;  
      
    {  
        int digit, roundup = 0;  
        // Rounding  
        // Look at the next digit  
        frac_part = (frac_part << 3) + (frac_part << 1);  
        digit = (frac_part >> (FRACTION_SIZE + 1));  
        if (digit > 5) roundup = 1;  
        else if (digit == 5) {  
            frac_part &= ((((FTOA_INT)1) << (FRACTION_SIZE + 1)) - 1); //0xFFFFFF;  
            if (frac_part != 0) roundup = 1;  
        }  
        if (roundup) {  
            char d;  
            int pos = ret - 1;  
            do {  
                d = outbuf[pos]; // last digit  
                if (d == '-') break;  
                if (d == '.') { pos--; continue; }  
                d++; outbuf[pos] = d;  
                if (d != ':') break;  
                outbuf[pos] = '0';  
                pos--;  
            } while (pos);  
        }  
    }  
  
    return ret;  
}  
  
#ifndef INCLUDED  
  
#undef FRACTION_SIZE  
#undef EXP_SIZE  
#undef FTOA_INT  
#define FRACTION_SIZE 52  
#define EXP_SIZE 11  
#define LF_t LF64_t  
#define FTOA_INT int64_t  
  
typedef union {  
    int64_t L;  
    double F;  
} LF64_t;  
  
int  
lftoa(double f, char *outbuf, int maxlen, int precision)   
#define INCLUDED  
#include "ftoa.c"  
#endif // INCLUDED 
