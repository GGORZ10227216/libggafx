//
// Created by orzgg on 2022/2/18.
//

#include <cstdint>
#include <bit_manipulate.h>

#include <ppu_enum.h>

#ifndef LIBGAFX_DISPCNT_H
#define LIBGAFX_DISPCNT_H

namespace gg_gfx {
    namespace DISP_CNT {
        static uint16_t GetDISPCNT(uint8_t const* ioReg)
        {
            // DISPCNT --> 0x04000000
            return *reinterpret_cast<uint16_t const*>(ioReg) ;
        } // GetDISPCNT()

        static int Mode(const uint16_t& dispcnt) {
            return gg_core::BitFieldValue<0, 3>(dispcnt) ;
        } // Priority()

        static bool DisplayFrameSelection(const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 4) ;
        } // TileBaseBlockNumber()

        static bool H_BlankObjProcessing(const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 5) ;
        } // Mosaic()

        static OBJ_DATA_FORMAT ObjFormatInVram (const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 6) ? 
                OBJ_DATA_FORMAT::LINEAR : OBJ_DATA_FORMAT::PLANE ;
        } // Mosaic()

        static bool ForcedBlank(const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 7) ;
        } // TileBlockSize()

        static int AllLayerStatus(const uint16_t& dispcnt) {
            return gg_core::BitFieldValue<8, 5>(dispcnt) ;
        } // MapBaseBlockNumber()

        static bool BgEnabled(const uint16_t& dispcnt, const int bgNum) {
            return gg_core::TestBit(dispcnt, bgNum + 8) ;
        } // BgEnabled()

        static bool ObjEnabled(const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 12) ;
        } // ObjEnabled()

        static bool Win0Enabled(const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 13) ;
        } // ObjEnabled()

        static bool Win1Enabled(const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 14) ;
        } // ObjEnabled()

        static bool WinObjEnabled(const uint16_t& dispcnt) {
            return gg_core::TestBit(dispcnt, 15) ;
        } // ObjEnabled()
    } // BG_CNT
}

#endif //LIBGAFX_DISPCNT_H
