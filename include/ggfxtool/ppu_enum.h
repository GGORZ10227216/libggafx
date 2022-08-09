//
// Created by orzgg on 2022/4/18.
//

#ifndef LIBGAFX_PPU_ENUM_H
#define LIBGAFX_PPU_ENUM_H

namespace gg_gfx {
    enum class BLD_MODE {NONE, NORMAL, BRIGHTEN, DARKEN} ;
    enum class BG_TYPE {TEXT, AFFINE} ;
    enum class OVERFLOW_BEHAVIOR {TRANSPARENT, WARP} ;
    enum class OBJ_DATA_FORMAT {PLANE, LINEAR} ;

    enum PIXEL_SIZE { NIBBLE, BYTE } ;
    enum LAYER_NAME {L_0, L_1, L_2, L_3, L_BACKDROP} ;
    enum WINNAME {WIN0 = 0, WIN1, OUT} ;

    using framebuffer_t = std::vector<uint16_t> ;
    struct WinRect {
        int x1, x2 ;
        int y1, y2 ;
    };
}

#endif //LIBGAFX_PPU_ENUM_H
