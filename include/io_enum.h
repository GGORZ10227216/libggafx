//
// Created by buildmachine on 2021-03-11.
//

#ifndef GGTEST_IO_ENUM_H
#define GGTEST_IO_ENUM_H

namespace gg_gfx {
    enum E_IOName {
        DISPCNT = 0x4000000,
        DISPSTAT = 0x4000004,
        VCOUNT = 0x4000006,
        BG0CNT = 0x4000008,
        BG1CNT = 0x400000a,
        BG2CNT = 0x400000c,
        BG3CNT = 0x400000e,
        BG0HOFS = 0x4000010,
        BG0VOFS = 0x4000012,
        BG1HOFS = 0x4000014,
        BG1VOFS = 0x4000016,
        BG2HOFS = 0x4000018,
        BG2VOFS = 0x400001a,
        BG3HOFS = 0x400001c,
        BG3VOFS = 0x400001e,
        BG2PA = 0x4000020,
        BG2PB = 0x4000022,
        BG2PC = 0x4000024,
        BG2PD = 0x4000026,
        BG2X = 0x4000028,
        BG2Y = 0x400002c,
        BG3PA = 0x4000030,
        BG3PB = 0x4000032,
        BG3PC = 0x4000034,
        BG3PD = 0x4000036,
        BG3X = 0x4000038,
        BG3Y = 0x400003c,
        WIN0H = 0x4000040,
        WIN1H = 0x4000042,
        WIN0V = 0x4000044,
        WIN1V = 0x4000046,
        WININ = 0x4000048,
        WINOUT = 0x400004a,
        MOSAIC = 0x400004c,
        BLDCNT = 0x4000050,
        BLDALPHA = 0x4000052,
        BLDY = 0x4000054
    };
}

#endif //GGTEST_IO_ENUM_H