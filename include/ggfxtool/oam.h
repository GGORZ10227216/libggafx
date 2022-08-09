//
// Created by 0rzgg on 2022/6/24.
//

#include <cstdint>
#include <bit_manipulate.h>
#include <ppu_enum.h>
#include <cassert>

#ifndef LIBGAFX_OAM_H
#define LIBGAFX_OAM_H

namespace gg_gfx {
    struct ObjRect {
        int w, h;
    };

    constexpr std::array<ObjRect, 12> objRectTable {
            ObjRect {.w = 8, .h = 8}, ObjRect {.w = 16, .h = 16}, ObjRect {.w = 32, .h = 32}, ObjRect {.w = 64, .h = 64},
            ObjRect {.w = 16, .h = 8}, ObjRect {.w = 32, .h = 8}, ObjRect {.w = 32, .h = 16}, ObjRect {.w = 64, .h = 32},
            ObjRect {.w = 8, .h = 16}, ObjRect {.w = 8, .h = 32}, ObjRect {.w = 16, .h = 32}, ObjRect {.w = 32, .h = 64}
    } ;

    struct ObjAttr {
        enum E_ObjType { NORMAL, ALPHA, WINDOW } ;
        enum E_ObjShape { SQR, H_REC, V_REC } ;
        uint16_t attr[ 4 ] ;

        // attr 0 area
        int y_crod() const { return attr[ 0 ] & 0xff ; }
        bool needTransform() const { return gg_core::TestBit(attr[ 0 ], 8); }
        bool needDoubleSize() const { return gg_core::TestBit(attr[ 0 ], 9); }
        E_ObjType type() { return static_cast<E_ObjType>(gg_core::BitFieldValue<10,2>(attr[0])); }
        bool needMosaic() { return gg_core::TestBit(attr[ 0 ], 12); }
        gg_gfx::PIXEL_SIZE pixelSize() const {
            return static_cast<gg_gfx::PIXEL_SIZE> (
                    gg_core::TestBit(attr[ 0 ], 13)
            );
        } // pixelSize()

        E_ObjShape objShape() const { return static_cast<E_ObjShape>(gg_core::BitFieldValue<14,2>(attr[ 0 ])); }

        // attr 1 area
        int x_cord() { return attr[ 1 ] & 0xff; }

        int transformMatrixNumber() {
            assert(("Attempt to use affine function on a normal obj!" || needTransform()));
            return gg_core::BitFieldValue<9,5>(attr[ 1 ]) ;
        } // transformMatrixNumber()

        bool H_Flip() const {
            assert(("Attempt to use h_flip function on an affine obj!" || !needTransform()));
            return gg_core::TestBit(attr[ 1 ], 12);
        } // H_Flip()

        bool V_Flip() const {
            assert(("Attempt to use v_flip function on an affine obj!" || !needTransform()));
            return gg_core::TestBit(attr[ 1 ], 13);
        } // V_Flip()

        int objSizeIdx() const { return gg_core::BitFieldValue<14,2>(attr[ 1 ]); }

        // attr2
        int characterName() const { return gg_core::BitFieldValue<0, 10>(attr[ 2 ]); }
        int priority() { return gg_core::BitFieldValue<10, 2>(attr[2]); }
        int paletteIdx() { return gg_core::BitFieldValue<12, 4>(attr[2]); }

        // special function to get obj's width and height
        ObjRect objRect() const { return objRectTable[ objShape()*4 + objSizeIdx() ] ; }
    };

    struct OAM {
        OAM(gg_raw_data &mem) :
            _mem(mem)
        {

        }

        ObjAttr& GetAttr(int attrNum) {
            assert(("Invalid attr number!" || attrNum < 128 && attrNum >= 0));
            return *reinterpret_cast<ObjAttr*> (_mem.data() + sizeof(ObjAttr)*attrNum) ;
        } // GetAttr()

        gg_raw_data &_mem ;
    };
}


#endif //LIBGAFX_OAM_H
