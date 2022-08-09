//
// Created by orzgg on 2022/7/11.
//

#include <ppu.h>
#include <bit_manipulate.h>

namespace gg_gfx {
    void ppu::DrawSpriteLine(uint8_t* vBuf, int spriteY, ObjAttr* attr) {
        const uint16_t disp_cnt = DISP_CNT::GetDISPCNT(rIoReg.data());
        OBJ_DATA_FORMAT charaAlignment = DISP_CNT::ObjFormatInVram(disp_cnt) ;
        ObjRect objRect = attr->objRect() ;

//        const uint8_t* spriteLineStart = [&]() {
//            if (charaAlignment == OBJ_DATA_FORMAT::LINEAR)
//                return GetSpriteCharacterAdd_1D(*attr, spriteY) ;
//            else
//                return GetSpriteCharacterAdd_2D(*attr, spriteY) ;
//        }();

        int spriteX = 0, tileY = spriteY % 8, step = 8 ; // X-Axis point to the sprite pixel that we are drawing

        if (attr->H_Flip()) {
            step = -8 ;
            spriteX = 56 ;
        } // if

        if (attr->V_Flip()) {
            spriteY = objRect.h - spriteY - 1;
        } // if

        for (int i = 0 ; i < objRect.w ; i += 8) {
            const uint8_t* spriteLineStart = [&]() {
                if (charaAlignment == OBJ_DATA_FORMAT::LINEAR)
                    return GetSpriteTile_1D(*attr, spriteX, spriteY) ;
                else
                    return GetSpriteTile_2D(*attr, spriteX, spriteY) ;
            }();

            uint64_t charaPixels ;

            if (attr->pixelSize() == NIBBLE) {
                charaPixels = Get4bppTilePixelColorIdx(
                        spriteLineStart, tileY,
                        attr->paletteIdx(),
                        attr->H_Flip(),
                        attr->V_Flip()
                ) ;
            } // if
            else {
                charaPixels = Get8bppTilePixelColorIdx(
                        spriteLineStart, tileY,
                        attr->H_Flip(),
                        attr->V_Flip()
                ) ;
            } // else

            for (int j = 0 ; j < 8 ; ++j) {
                int colorIdx = (charaPixels & ((uint64_t)0xff << (j * 8))) >> (j * 8) ;
                uint32_t pixelColor = gg_utility::RGB555_TO_RGB8888(ReadObjPaletteRaw(colorIdx));
                ((uint32_t*)vBuf)[ i + j ] = pixelColor ;
            } // for

            spriteX += step ;
        } // for
    } // ppu::DrawSpriteLine()
}