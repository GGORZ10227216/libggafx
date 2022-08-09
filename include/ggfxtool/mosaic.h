//
// Created by orzgg on 2022/3/3.
//

#include <cstdint>
#include <bit_manipulate.h>

#ifndef LIBGAFX_MOSAIC_H
#define LIBGAFX_MOSAIC_H

namespace gg_gfx {
    namespace MOSAIC_FX {
        static int BG_H_Size(uint16_t mosaic) {
            return gg_core::BitFieldValue<0, 4>(mosaic) ;
        } // BG_H_Size()

        static int BG_V_Size(uint16_t mosaic) {
            return gg_core::BitFieldValue<4, 4>(mosaic) ;
        } // BG_V_Size()

        static int OBJ_H_Size(uint16_t mosaic) {
            return gg_core::BitFieldValue<8, 4>(mosaic) ;
        } // OBJ_H_Size()

        static int OBJ_V_Size(uint16_t mosaic) {
            return gg_core::BitFieldValue<12, 4>(mosaic) ;
        } // OBJ_V_Size()
    } // MOSAIC_FX
}

#endif //LIBGAFX_MOSAIC_H
