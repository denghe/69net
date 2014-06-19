#ifndef NUMBITS_H
#define NUMBITS_H

namespace Lib3 {

// 用于 以适当的大小尾规则 来 直接读取 整数(1~8字节) 的每 字节/半字节/位 的数据
// 示例: var &p = *(Size4*)&i;  printf("%d %d %d %d", p.Byte0, p.Byte1, p.Byte2, p.Byte3);
namespace NumBits {

    #pragma pack(1)
    #ifdef __LE
        union Size1 {
            struct {
                Byte Byte0;
            };
            struct {
                Byte Byte0_L : 4;
                Byte Byte0_H : 4;
            };
            struct {
                Byte Bit0 : 1;
                Byte Bit1 : 1;
                Byte Bit2 : 1;
                Byte Bit3 : 1;
                Byte Bit4 : 1;
                Byte Bit5 : 1;
                Byte Bit6 : 1;
                Byte Bit7 : 1;
            };
        };
        union Size2 {
            struct {
                Byte Byte0;
                Byte Byte1;
            };
            struct {
                Byte Byte0_L : 4;
                Byte Byte0_H : 4;
                Byte Byte1_L : 4;
                Byte Byte1_H : 4;
            };
            struct {
                Byte Bit0  : 1;
                Byte Bit1  : 1;
                Byte Bit2  : 1;
                Byte Bit3  : 1;
                Byte Bit4  : 1;
                Byte Bit5  : 1;
                Byte Bit6  : 1;
                Byte Bit7  : 1;
                Byte Bit8  : 1;
                Byte Bit9  : 1;
                Byte Bit10 : 1;
                Byte Bit11 : 1;
                Byte Bit12 : 1;
                Byte Bit13 : 1;
                Byte Bit14 : 1;
                Byte Bit15 : 1;
            };
        };
        union Size4 {
            struct {
                Byte Byte0;
                Byte Byte1;
                Byte Byte2;
                Byte Byte3;
            };
            struct {
                Byte Byte0_L : 4;
                Byte Byte0_H : 4;
                Byte Byte1_L : 4;
                Byte Byte1_H : 4;
                Byte Byte2_L : 4;
                Byte Byte2_H : 4;
                Byte Byte3_L : 4;
                Byte Byte3_H : 4;
            };
            struct {
                Byte Bit0  : 1;
                Byte Bit1  : 1;
                Byte Bit2  : 1;
                Byte Bit3  : 1;
                Byte Bit4  : 1;
                Byte Bit5  : 1;
                Byte Bit6  : 1;
                Byte Bit7  : 1;
                Byte Bit8  : 1;
                Byte Bit9  : 1;
                Byte Bit10 : 1;
                Byte Bit11 : 1;
                Byte Bit12 : 1;
                Byte Bit13 : 1;
                Byte Bit14 : 1;
                Byte Bit15 : 1;
                Byte Bit16 : 1;
                Byte Bit17 : 1;
                Byte Bit18 : 1;
                Byte Bit19 : 1;
                Byte Bit20 : 1;
                Byte Bit21 : 1;
                Byte Bit22 : 1;
                Byte Bit23 : 1;
                Byte Bit24 : 1;
                Byte Bit25 : 1;
                Byte Bit26 : 1;
                Byte Bit27 : 1;
                Byte Bit28 : 1;
                Byte Bit29 : 1;
                Byte Bit30 : 1;
                Byte Bit31 : 1;
            };
        };
        union Size8 {
            struct {
                Byte Byte0;
                Byte Byte1;
                Byte Byte2;
                Byte Byte3;
                Byte Byte4;
                Byte Byte5;
                Byte Byte6;
                Byte Byte7;
            };
            struct {
                Byte Byte0_L : 4;
                Byte Byte0_H : 4;
                Byte Byte1_L : 4;
                Byte Byte1_H : 4;
                Byte Byte2_L : 4;
                Byte Byte2_H : 4;
                Byte Byte3_L : 4;
                Byte Byte3_H : 4;
                Byte Byte4_L : 4;
                Byte Byte4_H : 4;
                Byte Byte5_L : 4;
                Byte Byte5_H : 4;
                Byte Byte6_L : 4;
                Byte Byte6_H : 4;
                Byte Byte7_L : 4;
                Byte Byte7_H : 4;
            };
            struct {
                Byte Bit0 :  1;
                Byte Bit1 :  1;
                Byte Bit2 :  1;
                Byte Bit3 :  1;
                Byte Bit4 :  1;
                Byte Bit5 :  1;
                Byte Bit6 :  1;
                Byte Bit7 :  1;
                Byte Bit8 :  1;
                Byte Bit9 :  1;
                Byte Bit10 : 1;
                Byte Bit11 : 1;
                Byte Bit12 : 1;
                Byte Bit13 : 1;
                Byte Bit14 : 1;
                Byte Bit15 : 1;
                Byte Bit16 : 1;
                Byte Bit17 : 1;
                Byte Bit18 : 1;
                Byte Bit19 : 1;
                Byte Bit20 : 1;
                Byte Bit21 : 1;
                Byte Bit22 : 1;
                Byte Bit23 : 1;
                Byte Bit24 : 1;
                Byte Bit25 : 1;
                Byte Bit26 : 1;
                Byte Bit27 : 1;
                Byte Bit28 : 1;
                Byte Bit29 : 1;
                Byte Bit30 : 1;
                Byte Bit31 : 1;
                Byte Bit32 : 1;
                Byte Bit33 : 1;
                Byte Bit34 : 1;
                Byte Bit35 : 1;
                Byte Bit36 : 1;
                Byte Bit37 : 1;
                Byte Bit38 : 1;
                Byte Bit39 : 1;
                Byte Bit40 : 1;
                Byte Bit41 : 1;
                Byte Bit42 : 1;
                Byte Bit43 : 1;
                Byte Bit44 : 1;
                Byte Bit45 : 1;
                Byte Bit46 : 1;
                Byte Bit47 : 1;
                Byte Bit48 : 1;
                Byte Bit49 : 1;
                Byte Bit50 : 1;
                Byte Bit51 : 1;
                Byte Bit52 : 1;
                Byte Bit53 : 1;
                Byte Bit54 : 1;
                Byte Bit55 : 1;
                Byte Bit56 : 1;
                Byte Bit57 : 1;
                Byte Bit58 : 1;
                Byte Bit59 : 1;
                Byte Bit60 : 1;
                Byte Bit61 : 1;
                Byte Bit62 : 1;
                Byte Bit63 : 1;
            };
        };
    #else
        union Size1 {
            struct {
                Byte Byte0;
            };
            struct {
                Byte Byte0_H : 4;
                Byte Byte0_L : 4;
            };
            struct {
                Byte Bit7 : 1;
                Byte Bit6 : 1;
                Byte Bit5 : 1;
                Byte Bit4 : 1;
                Byte Bit3 : 1;
                Byte Bit2 : 1;
                Byte Bit1 : 1;
                Byte Bit0 : 1;
            };
        };
        union Size2 {
            struct {
                Byte Byte1;
                Byte Byte0;
            };
            struct {
                Byte Byte1_H : 4;
                Byte Byte1_L : 4;
                Byte Byte0_H : 4;
                Byte Byte0_L : 4;
            };
            struct {
                Byte Bit15 : 1;
                Byte Bit14 : 1;
                Byte Bit13 : 1;
                Byte Bit12 : 1;
                Byte Bit11 : 1;
                Byte Bit10 : 1;
                Byte Bit9  : 1;
                Byte Bit8  : 1;
                Byte Bit7 : 1;
                Byte Bit6 : 1;
                Byte Bit5 : 1;
                Byte Bit4 : 1;
                Byte Bit3 : 1;
                Byte Bit2 : 1;
                Byte Bit1 : 1;
                Byte Bit0 : 1;
            };
        };
        union Size4 {
            struct {
                Byte Byte3;
                Byte Byte2;
                Byte Byte1;
                Byte Byte0;
            };
            struct {
                Byte Byte3_H : 4;
                Byte Byte3_L : 4;
                Byte Byte2_H : 4;
                Byte Byte2_L : 4;
                Byte Byte1_H : 4;
                Byte Byte1_L : 4;
                Byte Byte0_H : 4;
                Byte Byte0_L : 4;
            };
            struct {
                Byte Bit31 : 1;
                Byte Bit30 : 1;
                Byte Bit29 : 1;
                Byte Bit28 : 1;
                Byte Bit27 : 1;
                Byte Bit26 : 1;
                Byte Bit25 : 1;
                Byte Bit24 : 1;
                Byte Bit23 : 1;
                Byte Bit22 : 1;
                Byte Bit21 : 1;
                Byte Bit10 : 1;
                Byte Bit19 : 1;
                Byte Bit18 : 1;
                Byte Bit17 : 1;
                Byte Bit16 : 1;
                Byte Bit15 : 1;
                Byte Bit14 : 1;
                Byte Bit13 : 1;
                Byte Bit12 : 1;
                Byte Bit11 : 1;
                Byte Bit10 : 1;
                Byte Bit9  : 1;
                Byte Bit8  : 1;
                Byte Bit7  : 1;
                Byte Bit6  : 1;
                Byte Bit5  : 1;
                Byte Bit4  : 1;
                Byte Bit3  : 1;
                Byte Bit2  : 1;
                Byte Bit1  : 1;
                Byte Bit0  : 1;
            };
        };
        union Size8 {
            struct {
                Byte Byte7;
                Byte Byte6;
                Byte Byte5;
                Byte Byte4;
                Byte Byte3;
                Byte Byte2;
                Byte Byte1;
                Byte Byte0;
            };
            struct {
                Byte Byte7_H : 4;
                Byte Byte7_L : 4;
                Byte Byte6_H : 4;
                Byte Byte6_L : 4;
                Byte Byte5_H : 4;
                Byte Byte5_L : 4;
                Byte Byte4_H : 4;
                Byte Byte4_L : 4;
                Byte Byte3_H : 4;
                Byte Byte3_L : 4;
                Byte Byte2_H : 4;
                Byte Byte2_L : 4;
                Byte Byte1_H : 4;
                Byte Byte1_L : 4;
                Byte Byte0_H : 4;
                Byte Byte0_L : 4;
            };
            struct {
                Byte Bit63 : 1;
                Byte Bit62 : 1;
                Byte Bit61 : 1;
                Byte Bit60 : 1;
                Byte Bit59 : 1;
                Byte Bit58 : 1;
                Byte Bit57 : 1;
                Byte Bit56 : 1;
                Byte Bit55 : 1;
                Byte Bit54 : 1;
                Byte Bit53 : 1;
                Byte Bit52 : 1;
                Byte Bit51 : 1;
                Byte Bit40 : 1;
                Byte Bit49 : 1;
                Byte Bit48 : 1;
                Byte Bit47 : 1;
                Byte Bit46 : 1;
                Byte Bit45 : 1;
                Byte Bit44 : 1;
                Byte Bit43 : 1;
                Byte Bit42 : 1;
                Byte Bit41 : 1;
                Byte Bit40 : 1;
                Byte Bit39 : 1;
                Byte Bit38 : 1;
                Byte Bit37 : 1;
                Byte Bit36 : 1;
                Byte Bit35 : 1;
                Byte Bit34 : 1;
                Byte Bit33 : 1;
                Byte Bit32 : 1;
                Byte Bit31 : 1;
                Byte Bit30 : 1;
                Byte Bit29 : 1;
                Byte Bit28 : 1;
                Byte Bit27 : 1;
                Byte Bit26 : 1;
                Byte Bit25 : 1;
                Byte Bit24 : 1;
                Byte Bit23 : 1;
                Byte Bit22 : 1;
                Byte Bit21 : 1;
                Byte Bit10 : 1;
                Byte Bit19 : 1;
                Byte Bit18 : 1;
                Byte Bit17 : 1;
                Byte Bit16 : 1;
                Byte Bit15 : 1;
                Byte Bit14 : 1;
                Byte Bit13 : 1;
                Byte Bit12 : 1;
                Byte Bit11 : 1;
                Byte Bit10 : 1;
                Byte Bit9  : 1;
                Byte Bit8  : 1;
                Byte Bit7  : 1;
                Byte Bit6  : 1;
                Byte Bit5  : 1;
                Byte Bit4  : 1;
                Byte Bit3  : 1;
                Byte Bit2  : 1;
                Byte Bit1  : 1;
                Byte Bit0  : 1;
            };
        };
    #endif
    #pragma pack()


}}


#endif
