//
// Created by orzgg on 2022/2/18.
//

#include <cstdint>
#include <bit_manipulate.h>

#include <ppu_enum.h>

#ifndef LIBGAFX_BGCNT_H
#define LIBGAFX_BGCNT_H

namespace gg_gfx {
    namespace BG_CNT {
        static uint16_t GetBGCNT(uint8_t const* ioReg, int layerNum)
        {
            auto* BGCNT_base = reinterpret_cast<uint16_t const*>(ioReg + BG0CNT - 0x0400'0000);
            return BGCNT_base[layerNum] ;
        } // GetBGCNT()

        static int Priority(const uint16_t& bgcnt) {
            return gg_core::BitFieldValue<0, 2>(bgcnt) ;
        } // Priority()

        static int TileBaseBlockNumber(const uint16_t& bgcnt) {
            return gg_core::BitFieldValue<2, 2>(bgcnt) ;
        } // TileBaseBlockNumber()

        static bool Mosaic(const uint16_t& bgcnt) {
            return gg_core::TestBit(bgcnt, 6) ;
        } // Mosaic()

        static PIXEL_SIZE PixelSize(const uint16_t& bgcnt) {
            return gg_core::TestBit(bgcnt, 7) ? BYTE : NIBBLE ;
        } // Mosaic()

        static int TileBlockSize(const uint16_t& bgcnt, const BG_TYPE bgType) {
            if (bgType == BG_TYPE::AFFINE)
                return 0x40 ;
            else
                return gg_core::TestBit(bgcnt, 7) ? 0x40 : 0x20;
        } // TileBlockSize()

        static int MapBaseBlockNumber(const uint16_t& bgcnt) {
            return gg_core::BitFieldValue<8, 5>(bgcnt) ;
        } // MapBaseBlockNumber()

        static OVERFLOW_BEHAVIOR OverflowBehavior(const uint16_t& bgcnt) {
            return static_cast<OVERFLOW_BEHAVIOR> (
                gg_core::TestBit(bgcnt, 13)
            );
        } // OverflowBehavior()

        static int ScreenSizeID(const uint16_t& bgcnt) {
            return gg_core::BitFieldValue<14, 2>(bgcnt);
        } // ScreenSizeID()
    } // BG_CNT
}

#endif //LIBGAFX_BGCNT_H
