#ifndef _BITMASK_H_
#define _BITMASK_H_


// 主用于 指针硬转引用 以适当的大小尾规则 来 直接读取 整数(1~8字节) 的每 字节/半字节/位 的数据
// 示例: var &p = *(UInt32*)&i;  printf("%d %d %d %d", p.Byte0, p.Byte1, p.Byte2, p.Byte3);
namespace BitMask {

#pragma pack(1)
#ifdef __LE
union UInt8 {
    struct {
        uint8 Byte0;
    };
    struct {
        uint8 Byte0_L : 4;
        uint8 Byte0_H : 4;
    };
    struct {
        uint8 Bit0 : 1;
        uint8 Bit1 : 1;
        uint8 Bit2 : 1;
        uint8 Bit3 : 1;
        uint8 Bit4 : 1;
        uint8 Bit5 : 1;
        uint8 Bit6 : 1;
        uint8 Bit7 : 1;
    };
};
union UInt16 {
    struct {
        uint8 Byte0;
        uint8 Byte1;
    };
    struct {
        uint8 Byte0_L : 4;
        uint8 Byte0_H : 4;
        uint8 Byte1_L : 4;
        uint8 Byte1_H : 4;
    };
    struct {
        uint8 Bit0  : 1;
        uint8 Bit1  : 1;
        uint8 Bit2  : 1;
        uint8 Bit3  : 1;
        uint8 Bit4  : 1;
        uint8 Bit5  : 1;
        uint8 Bit6  : 1;
        uint8 Bit7  : 1;
        uint8 Bit8  : 1;
        uint8 Bit9  : 1;
        uint8 Bit10 : 1;
        uint8 Bit11 : 1;
        uint8 Bit12 : 1;
        uint8 Bit13 : 1;
        uint8 Bit14 : 1;
        uint8 Bit15 : 1;
    };
};
union UInt32 {
    struct {
        uint8 Byte0;
        uint8 Byte1;
        uint8 Byte2;
        uint8 Byte3;
    };
    struct {
        uint8 Byte0_L : 4;
        uint8 Byte0_H : 4;
        uint8 Byte1_L : 4;
        uint8 Byte1_H : 4;
        uint8 Byte2_L : 4;
        uint8 Byte2_H : 4;
        uint8 Byte3_L : 4;
        uint8 Byte3_H : 4;
    };
    struct {
        uint8 Bit0  : 1;
        uint8 Bit1  : 1;
        uint8 Bit2  : 1;
        uint8 Bit3  : 1;
        uint8 Bit4  : 1;
        uint8 Bit5  : 1;
        uint8 Bit6  : 1;
        uint8 Bit7  : 1;
        uint8 Bit8  : 1;
        uint8 Bit9  : 1;
        uint8 Bit10 : 1;
        uint8 Bit11 : 1;
        uint8 Bit12 : 1;
        uint8 Bit13 : 1;
        uint8 Bit14 : 1;
        uint8 Bit15 : 1;
        uint8 Bit16 : 1;
        uint8 Bit17 : 1;
        uint8 Bit18 : 1;
        uint8 Bit19 : 1;
        uint8 Bit20 : 1;
        uint8 Bit21 : 1;
        uint8 Bit22 : 1;
        uint8 Bit23 : 1;
        uint8 Bit24 : 1;
        uint8 Bit25 : 1;
        uint8 Bit26 : 1;
        uint8 Bit27 : 1;
        uint8 Bit28 : 1;
        uint8 Bit29 : 1;
        uint8 Bit30 : 1;
        uint8 Bit31 : 1;
    };
};
union UInt64 {
    struct {
        uint8 Byte0;
        uint8 Byte1;
        uint8 Byte2;
        uint8 Byte3;
        uint8 Byte4;
        uint8 Byte5;
        uint8 Byte6;
        uint8 Byte7;
    };
    struct {
        uint8 Byte0_L : 4;
        uint8 Byte0_H : 4;
        uint8 Byte1_L : 4;
        uint8 Byte1_H : 4;
        uint8 Byte2_L : 4;
        uint8 Byte2_H : 4;
        uint8 Byte3_L : 4;
        uint8 Byte3_H : 4;
        uint8 Byte4_L : 4;
        uint8 Byte4_H : 4;
        uint8 Byte5_L : 4;
        uint8 Byte5_H : 4;
        uint8 Byte6_L : 4;
        uint8 Byte6_H : 4;
        uint8 Byte7_L : 4;
        uint8 Byte7_H : 4;
    };
    struct {
        uint8 Bit0 :  1;
        uint8 Bit1 :  1;
        uint8 Bit2 :  1;
        uint8 Bit3 :  1;
        uint8 Bit4 :  1;
        uint8 Bit5 :  1;
        uint8 Bit6 :  1;
        uint8 Bit7 :  1;
        uint8 Bit8 :  1;
        uint8 Bit9 :  1;
        uint8 Bit10 : 1;
        uint8 Bit11 : 1;
        uint8 Bit12 : 1;
        uint8 Bit13 : 1;
        uint8 Bit14 : 1;
        uint8 Bit15 : 1;
        uint8 Bit16 : 1;
        uint8 Bit17 : 1;
        uint8 Bit18 : 1;
        uint8 Bit19 : 1;
        uint8 Bit20 : 1;
        uint8 Bit21 : 1;
        uint8 Bit22 : 1;
        uint8 Bit23 : 1;
        uint8 Bit24 : 1;
        uint8 Bit25 : 1;
        uint8 Bit26 : 1;
        uint8 Bit27 : 1;
        uint8 Bit28 : 1;
        uint8 Bit29 : 1;
        uint8 Bit30 : 1;
        uint8 Bit31 : 1;
        uint8 Bit32 : 1;
        uint8 Bit33 : 1;
        uint8 Bit34 : 1;
        uint8 Bit35 : 1;
        uint8 Bit36 : 1;
        uint8 Bit37 : 1;
        uint8 Bit38 : 1;
        uint8 Bit39 : 1;
        uint8 Bit40 : 1;
        uint8 Bit41 : 1;
        uint8 Bit42 : 1;
        uint8 Bit43 : 1;
        uint8 Bit44 : 1;
        uint8 Bit45 : 1;
        uint8 Bit46 : 1;
        uint8 Bit47 : 1;
        uint8 Bit48 : 1;
        uint8 Bit49 : 1;
        uint8 Bit50 : 1;
        uint8 Bit51 : 1;
        uint8 Bit52 : 1;
        uint8 Bit53 : 1;
        uint8 Bit54 : 1;
        uint8 Bit55 : 1;
        uint8 Bit56 : 1;
        uint8 Bit57 : 1;
        uint8 Bit58 : 1;
        uint8 Bit59 : 1;
        uint8 Bit60 : 1;
        uint8 Bit61 : 1;
        uint8 Bit62 : 1;
        uint8 Bit63 : 1;
    };
};
#else
union UInt8 {
    struct {
        uint8 Byte0;
    };
    struct {
        uint8 Byte0_H : 4;
        uint8 Byte0_L : 4;
    };
    struct {
        uint8 Bit7 : 1;
        uint8 Bit6 : 1;
        uint8 Bit5 : 1;
        uint8 Bit4 : 1;
        uint8 Bit3 : 1;
        uint8 Bit2 : 1;
        uint8 Bit1 : 1;
        uint8 Bit0 : 1;
    };
};
union UInt16 {
    struct {
        uint8 Byte1;
        uint8 Byte0;
    };
    struct {
        uint8 Byte1_H : 4;
        uint8 Byte1_L : 4;
        uint8 Byte0_H : 4;
        uint8 Byte0_L : 4;
    };
    struct {
        uint8 Bit15 : 1;
        uint8 Bit14 : 1;
        uint8 Bit13 : 1;
        uint8 Bit12 : 1;
        uint8 Bit11 : 1;
        uint8 Bit10 : 1;
        uint8 Bit9  : 1;
        uint8 Bit8  : 1;
        uint8 Bit7 : 1;
        uint8 Bit6 : 1;
        uint8 Bit5 : 1;
        uint8 Bit4 : 1;
        uint8 Bit3 : 1;
        uint8 Bit2 : 1;
        uint8 Bit1 : 1;
        uint8 Bit0 : 1;
    };
};
union UInt32 {
    struct {
        uint8 Byte3;
        uint8 Byte2;
        uint8 Byte1;
        uint8 Byte0;
    };
    struct {
        uint8 Byte3_H : 4;
        uint8 Byte3_L : 4;
        uint8 Byte2_H : 4;
        uint8 Byte2_L : 4;
        uint8 Byte1_H : 4;
        uint8 Byte1_L : 4;
        uint8 Byte0_H : 4;
        uint8 Byte0_L : 4;
    };
    struct {
        uint8 Bit31 : 1;
        uint8 Bit30 : 1;
        uint8 Bit29 : 1;
        uint8 Bit28 : 1;
        uint8 Bit27 : 1;
        uint8 Bit26 : 1;
        uint8 Bit25 : 1;
        uint8 Bit24 : 1;
        uint8 Bit23 : 1;
        uint8 Bit22 : 1;
        uint8 Bit21 : 1;
        uint8 Bit10 : 1;
        uint8 Bit19 : 1;
        uint8 Bit18 : 1;
        uint8 Bit17 : 1;
        uint8 Bit16 : 1;
        uint8 Bit15 : 1;
        uint8 Bit14 : 1;
        uint8 Bit13 : 1;
        uint8 Bit12 : 1;
        uint8 Bit11 : 1;
        uint8 Bit10 : 1;
        uint8 Bit9  : 1;
        uint8 Bit8  : 1;
        uint8 Bit7  : 1;
        uint8 Bit6  : 1;
        uint8 Bit5  : 1;
        uint8 Bit4  : 1;
        uint8 Bit3  : 1;
        uint8 Bit2  : 1;
        uint8 Bit1  : 1;
        uint8 Bit0  : 1;
    };
};
union UInt64 {
    struct {
        uint8 Byte7;
        uint8 Byte6;
        uint8 Byte5;
        uint8 Byte4;
        uint8 Byte3;
        uint8 Byte2;
        uint8 Byte1;
        uint8 Byte0;
    };
    struct {
        uint8 Byte7_H : 4;
        uint8 Byte7_L : 4;
        uint8 Byte6_H : 4;
        uint8 Byte6_L : 4;
        uint8 Byte5_H : 4;
        uint8 Byte5_L : 4;
        uint8 Byte4_H : 4;
        uint8 Byte4_L : 4;
        uint8 Byte3_H : 4;
        uint8 Byte3_L : 4;
        uint8 Byte2_H : 4;
        uint8 Byte2_L : 4;
        uint8 Byte1_H : 4;
        uint8 Byte1_L : 4;
        uint8 Byte0_H : 4;
        uint8 Byte0_L : 4;
    };
    struct {
        uint8 Bit63 : 1;
        uint8 Bit62 : 1;
        uint8 Bit61 : 1;
        uint8 Bit60 : 1;
        uint8 Bit59 : 1;
        uint8 Bit58 : 1;
        uint8 Bit57 : 1;
        uint8 Bit56 : 1;
        uint8 Bit55 : 1;
        uint8 Bit54 : 1;
        uint8 Bit53 : 1;
        uint8 Bit52 : 1;
        uint8 Bit51 : 1;
        uint8 Bit40 : 1;
        uint8 Bit49 : 1;
        uint8 Bit48 : 1;
        uint8 Bit47 : 1;
        uint8 Bit46 : 1;
        uint8 Bit45 : 1;
        uint8 Bit44 : 1;
        uint8 Bit43 : 1;
        uint8 Bit42 : 1;
        uint8 Bit41 : 1;
        uint8 Bit40 : 1;
        uint8 Bit39 : 1;
        uint8 Bit38 : 1;
        uint8 Bit37 : 1;
        uint8 Bit36 : 1;
        uint8 Bit35 : 1;
        uint8 Bit34 : 1;
        uint8 Bit33 : 1;
        uint8 Bit32 : 1;
        uint8 Bit31 : 1;
        uint8 Bit30 : 1;
        uint8 Bit29 : 1;
        uint8 Bit28 : 1;
        uint8 Bit27 : 1;
        uint8 Bit26 : 1;
        uint8 Bit25 : 1;
        uint8 Bit24 : 1;
        uint8 Bit23 : 1;
        uint8 Bit22 : 1;
        uint8 Bit21 : 1;
        uint8 Bit10 : 1;
        uint8 Bit19 : 1;
        uint8 Bit18 : 1;
        uint8 Bit17 : 1;
        uint8 Bit16 : 1;
        uint8 Bit15 : 1;
        uint8 Bit14 : 1;
        uint8 Bit13 : 1;
        uint8 Bit12 : 1;
        uint8 Bit11 : 1;
        uint8 Bit10 : 1;
        uint8 Bit9  : 1;
        uint8 Bit8  : 1;
        uint8 Bit7  : 1;
        uint8 Bit6  : 1;
        uint8 Bit5  : 1;
        uint8 Bit4  : 1;
        uint8 Bit3  : 1;
        uint8 Bit2  : 1;
        uint8 Bit1  : 1;
        uint8 Bit0  : 1;
    };
};
#endif
#pragma pack()


}



#endif
