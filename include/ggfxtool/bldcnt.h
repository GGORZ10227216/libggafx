//
// Created by orzgg on 2022/2/18.
//

#include <cstdint>
#include <bit_manipulate.h>

#include <ppu_enum.h>

#ifndef LIBGAFX_BLDCNT_H
#define LIBGAFX_BLDCNT_H

namespace gg_gfx {
    namespace BLD_CNT {
        static uint16_t GetBLDCNT(uint8_t const* ioReg)
        {
            // BLDCNT --> 0x04000050
            return *reinterpret_cast<uint16_t const*>(ioReg + 0x50) ;
        } // GetBLDCNT()

        static BLD_MODE Mode(uint8_t const* ioReg) {
            return static_cast<BLD_MODE>(gg_core::BitFieldValue<6, 2>(GetBLDCNT(ioReg))) ;
        } // Mode()

        static int FirstTarget(uint8_t const* ioReg) {
            auto bldcnt = *reinterpret_cast<uint16_t const*>(ioReg + BLDCNT - 0x0400'0000);
            return bldcnt & 0x3f ;
        } // FirstTarget()

        static int SecondTarget(uint8_t const* ioReg) {
            auto bldcnt = *reinterpret_cast<uint16_t const*>(ioReg + BLDCNT - 0x0400'0000);
            return (bldcnt >> 8) & 0x3f ;
        } // SecondTarget()

        static int EVA(uint8_t const* ioReg) {
            auto bldalpha = *reinterpret_cast<uint16_t const*>(ioReg + BLDALPHA - 0x0400'0000);
            int rawEVA = bldalpha & 0x1f ;
            return rawEVA > 0xf ? 0x10 : rawEVA ;
        } // EVA()

        static int EVB(uint8_t const* ioReg) {
            auto bldalpha = *reinterpret_cast<uint16_t const*>(ioReg + BLDALPHA - 0x0400'0000);
            int rawEVB = (bldalpha >> 8) & 0x1f ;
            return rawEVB > 0xf ? 0x10 : rawEVB ;
        } // EVB()

        static int EVY(uint8_t const* ioReg) {
            auto bldy = *reinterpret_cast<uint16_t const*>(ioReg + BLDY - 0x0400'0000);
            int rawEVY = bldy & 0x1f ;
            return rawEVY > 0xf ? 0x10 : rawEVY ;
        } // EVY()
    } // BLD_CNT
}

#endif //LIBGAFX_BLDCNT_H
