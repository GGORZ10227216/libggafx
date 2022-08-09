//
// Created by orzgg on 2022/3/3.
//

#include <io_enum.h>

#include <cassert>
#include <cstdint>
#include <bit_manipulate.h>

#include <ppu_enum.h>

#ifndef LIBGAFX_AFFINE_BG_H
#define LIBGAFX_AFFINE_BG_H

namespace gg_gfx {
    namespace AFFBG {
        static int GetBGX(int layerNum, const uint8_t* ioReg) {
            assert(("Invalid affine layer number", layerNum == 2 || layerNum == 3)) ;
            const int offset = (BG2X - 0x0400'0000) + (layerNum-2)*4 ;
            return *reinterpret_cast<uint32_t const*>(ioReg + offset);
        } // GetBGX()

        static int GetBGY(int layerNum, const uint8_t* ioReg) {
            assert(("Invalid affine layer number", layerNum == 2 || layerNum == 3)) ;
            const int offset = (BG2Y - 0x0400'0000) + (layerNum-2)*4 ;
            return *reinterpret_cast<uint32_t const*>(ioReg + offset);
        } // GetBGX()

        static int GetBG_PA(int layerNum, const uint8_t* ioReg) {
            assert(("Invalid affine layer number", layerNum == 2 || layerNum == 3)) ;
            const int offset = (BG2PA - 0x0400'0000) + (layerNum-2)*0x10 ;
            return *reinterpret_cast<uint16_t const*>(ioReg + offset);
        } // GetBGX()

        static int GetBG_PB(int layerNum, const uint8_t* ioReg) {
            assert(("Invalid affine layer number", layerNum == 2 || layerNum == 3)) ;
            const int offset = (BG2PB - 0x0400'0000) + (layerNum-2)*0x10 ;
            return *reinterpret_cast<uint16_t const*>(ioReg + offset);
        } // GetBGX()

        static int GetBG_PC(int layerNum, const uint8_t* ioReg) {
            assert(("Invalid affine layer number", layerNum == 2 || layerNum == 3)) ;
            const int offset = (BG2PC - 0x0400'0000) + (layerNum-2)*0x10 ;
            return *reinterpret_cast<uint16_t const*>(ioReg + offset);
        } // GetBGX()

        static int GetBG_PD(int layerNum, const uint8_t* ioReg) {
            assert(("Invalid affine layer number", layerNum == 2 || layerNum == 3)) ;
            const int offset = (BG2PD - 0x0400'0000) + (layerNum-2)*0x10 ;
            return *reinterpret_cast<uint16_t const*>(ioReg + offset);
        } // GetBGX()
    } // AFFBG
}

#endif //LIBGAFX_AFFINE_BG_H
