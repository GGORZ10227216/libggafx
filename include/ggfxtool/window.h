//
// Created by orzgg on 2022/3/3.
//

#include <io_enum.h>

#include <cassert>
#include <cstdint>
#include <bit_manipulate.h>

#include <ppu_enum.h>

#ifndef LIBGAFX_WINDOW_H
#define LIBGAFX_WINDOW_H

namespace gg_gfx {
    namespace WINDOW {
        constexpr uint16_t maxValue = 240 ;

        static int WIN0_X1(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN0H - 0x0400'0000);
            return std::min(gg_core::BitFieldValue<8, 8>(*base), maxValue) ;
        } // WIN0_X1()

        static int WIN0_X2(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN0H - 0x0400'0000);
            return gg_core::BitFieldValue<0, 8>(*base) ;
        } // WIN0_X2()

        static int WIN1_X1(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN1H - 0x0400'0000);
            return gg_core::BitFieldValue<8, 8>(*base) ;
        } // WIN1_X1()

        static int WIN1_X2(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN1H - 0x0400'0000);
            return gg_core::BitFieldValue<0, 8>(*base) ;
        } // WIN1_X2()

        static int WIN0_Y1(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN0V - 0x0400'0000);
            return gg_core::BitFieldValue<8, 8>(*base) ;
        } // WIN0_Y1()

        static int WIN0_Y2(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN0V - 0x0400'0000);
            return gg_core::BitFieldValue<0, 8>(*base) ;
        } // WIN0_Y2()

        static int WIN1_Y1(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN1V - 0x0400'0000);
            return gg_core::BitFieldValue<8, 8>(*base) ;
        } // WIN1_Y1()

        static int WIN1_Y2(uint8_t const* ioReg) {
            auto* base = reinterpret_cast<uint16_t const*>(ioReg + WIN1V - 0x0400'0000);
            return gg_core::BitFieldValue<0, 8>(*base) ;
        } // WIN1_Y2()

        static uint16_t GetWinIN(uint8_t const* ioReg) {
            return reinterpret_cast<const uint16_t&>(ioReg[WININ - 0x0400'0000]);
        } // GetWinIN()

        static bool WIN0_LAYER_ENABLED(uint8_t const* ioReg, int layerNum) {
            const uint16_t winIn = reinterpret_cast<const uint16_t&>(ioReg[WININ - 0x0400'0000]);
            assert(("Invalid layer number", layerNum <= 3));

            return gg_core::TestBit(winIn, layerNum);
        } // WIN0_LAYER_ENABLE()

        static bool WIN1_LAYER_ENABLED(uint8_t const* ioReg, int layerNum) {
            const uint16_t winIn = reinterpret_cast<const uint16_t&>(ioReg[WININ - 0x0400'0000]);
            assert(("Invalid layer number", layerNum <= 3));

            return gg_core::TestBit(winIn >> 8, layerNum);
        } // WIN0_LAYER_ENABLE()

        static bool WINOUT_LAYER_ENABLED(uint8_t const* ioReg, int layerNum) {
            const uint16_t winOut = reinterpret_cast<const uint16_t&>(ioReg[WINOUT - 0x0400'0000]);
            assert(("Invalid layer number", layerNum <= 3));

            return gg_core::TestBit(winOut, layerNum);
        } // WIN0_LAYER_ENABLE()

        static bool WIN0_SFX_ENABLED(uint8_t const* ioReg) {
            const uint16_t winIn = reinterpret_cast<const uint16_t&>(ioReg[WININ - 0x0400'0000]);
            return gg_core::TestBit(winIn, 5);
        } // WIN0_SFX_ENABLED()

        static bool WIN1_SFX_ENABLED(uint8_t const* ioReg) {
            const uint16_t winIn = reinterpret_cast<const uint16_t&>(ioReg[WININ - 0x0400'0000]);
            return gg_core::TestBit(winIn >> 8, 5);
        } // WIN0_SFX_ENABLED()

        static bool WININ_SFX_ENABLED(uint8_t const* ioReg, int winNum) {
            assert(("Invalid window number", winNum <= 1));
            const uint16_t winIn = reinterpret_cast<const uint16_t&>(ioReg[WININ - 0x0400'0000]);
            return gg_core::TestBit(winIn >> 8*winNum, 5);
        } // WIN0_SFX_ENABLED()

        static bool WINOUT_SFX_ENABLED(uint8_t const* ioReg) {
            const uint16_t winOut = reinterpret_cast<const uint16_t&>(ioReg[WINOUT - 0x0400'0000]);
            return gg_core::TestBit(winOut, 5);
        } // WIN0_SFX_ENABLED()

        static bool WIN_LAYER_ENABLED(uint8_t const* ioReg, int winNum, int layerNum) {
            const uint16_t winIn = reinterpret_cast<const uint16_t&>(ioReg[WININ - 0x0400'0000]);
            assert(("Invalid window number", winNum <= 1));
            assert(("Invalid layer number", layerNum <= 3));

            return gg_core::TestBit(winIn >> (winNum << 3), layerNum);
        } // WIN0_LAYER_ENABLE()

        static WinRect GetWindowRect(uint8_t const* ioReg, int winNum) {
            WinRect result ;
            switch (winNum) {
                case 0:
                    result.x1 = WIN0_X1(ioReg) ;
                    result.x2 = WIN0_X2(ioReg) ;
                    result.y1 = WIN0_Y1(ioReg) ;
                    result.y2 = WIN0_Y2(ioReg) ;
                    break ;
                case 1:
                    result.x1 = WIN1_X1(ioReg) ;
                    result.x2 = WIN1_X2(ioReg) ;
                    result.y1 = WIN1_Y1(ioReg) ;
                    result.y2 = WIN1_Y2(ioReg) ;
                    break ;
                default :
                    assert("Invalid window number") ;
            } // switch()

            return result ;
        } // GetWindowRect()
    } // MOSAIC_FX
}

#endif //LIBGAFX_WINDOW_H
