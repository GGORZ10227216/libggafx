//
// Created by orzgg on 2022/2/18.
//

#include <ppu.h>
#include <bit_manipulate.h>
#include <affine_bg.h>

#include <utility>

#include <chrono>

namespace gg_gfx {
    namespace TextMode {
        static constexpr std::array<std::pair<int, int>, 4> affineModeBgSize = {
                std::make_pair(128, 128),
                std::make_pair(256, 256),
                std::make_pair(512, 512),
                std::make_pair(1024, 1024)
        };
    } // TextMode

    int q_mul(int32_t a, int16_t b)
    {
        int32_t temp;

        temp = (int32_t)a * (int32_t)b; // result type is operand's type
        // Rounding; mid values are rounded up
        temp += (temp >> 7) & 1;

//        std::cout << "\t" << a << "x" << b << "=" << (temp >>8) << std::endl ;
        return temp >> 8;
    }

    int mod(int a, int b) { return (a % b + b) % b; }

    void ppu::DrawAffineBackgroundScanline(int layerNum, int rowNum) {
        assert(("Invalid affine layer number", layerNum == 2 || layerNum == 3)) ;

        const int bgcntAddr = BG0CNT - 0x0400'0000 + (layerNum << 1);

        const uint16_t bgcnt = (uint16_t &) rIoReg[bgcntAddr];
        const int bgx = AFFBG::GetBGX(layerNum, rIoReg.data()) ; // Notice that it's a fixed point number
        const int bgy = AFFBG::GetBGY(layerNum, rIoReg.data()) ; // Notice that it's a fixed point number

        const int16_t bg_pa = AFFBG::GetBG_PA(layerNum, rIoReg.data()) ; // Notice that it's a fixed point number
        const int16_t bg_pb = AFFBG::GetBG_PB(layerNum, rIoReg.data()) ; // Notice that it's a fixed point number
        const int16_t bg_pc = AFFBG::GetBG_PC(layerNum, rIoReg.data()) ; // Notice that it's a fixed point number
        const int16_t bg_pd = AFFBG::GetBG_PD(layerNum, rIoReg.data()) ; // Notice that it's a fixed point number

//        const int16_t bg_pa = 1 << 8 ; // Notice that it's a fixed point number
//        const int16_t bg_pb = 0 ; // Notice that it's a fixed point number
//        const int16_t bg_pc = 0 ; // Notice that it's a fixed point number
//        const int16_t bg_pd = 1 << 8 ; // Notice that it's a fixed point number

        const int fullBgWidth = TextMode::affineModeBgSize[BG_CNT::ScreenSizeID(bgcnt)].first;
        const int fullBgHeight = TextMode::affineModeBgSize[BG_CNT::ScreenSizeID(bgcnt)].second;

        const int mapBaseBlkNumber = BG_CNT::MapBaseBlockNumber(bgcnt);
        const int tileBaseBlkNumber = BG_CNT::TileBaseBlockNumber(bgcnt);

        uint8_t const *mapAddrBase = rVram.data() + 0x800*mapBaseBlkNumber;
        uint8_t const *tileAddrBase = rVram.data() + tileBaseBlkNumber * 0x4000;

        const int screenY = rowNum ;

        bgLineSize = 240 ;
        bgLineOffset = 0 ;

        for (int screenX = 0 ; screenX < 240 ; ++screenX) {
            // fixme: +bgx or -bgx?
            int textureX = q_mul(screenX << 8, bg_pa) + q_mul(screenY << 8, bg_pb) ;
            int textureY = q_mul(screenX << 8, bg_pc) + q_mul(screenY << 8, bg_pd) ;

//            int textureX = screenX;
//            int textureY = screenY;

            /* Shift out the fractional part */
//            std::cout << "\t\t" << textureX << "," << textureY << std::endl ;
            textureX = (textureX + bgx) >> 8;
            textureY = (textureY + bgy) >> 8;

//            textureY += 50;
//            std::cout << "\t\t" << textureX << "|" << textureY << std::endl ;

            bool xOverflow = textureX >= fullBgWidth || textureX < 0 ;
            bool yOverflow = textureY >= fullBgHeight || textureY < 0 ;

            if (xOverflow || yOverflow) {
                if (BG_CNT::OverflowBehavior(bgcnt) == OVERFLOW_BEHAVIOR::WARP) {
                    textureX = mod(textureX, fullBgWidth) ;
                    textureY = mod(textureY, fullBgHeight) ;
                } // if
                else {
                    // Write backdrop directly, no need to load map & tile data.
                    bgLineBuf[ layerNum ][ screenX ] = 0x0 ;
                    continue ;
                } // else
            } // if

            int mapX = textureX / 8, mapY = textureY / 8 ;
            int tileX = textureX % 8, tileY = textureY % 8 ;

            // fullBgWidth/8 to get number of map entity in per map row
            const int mapIdx = (fullBgWidth >> 3)*mapY + mapX;
            const uint8_t tileNum = *(mapAddrBase + mapIdx);
            uint8_t const *tileDataPtr = tileAddrBase + tileNum * 0x40;

            bgLineBuf[ layerNum ][ screenX ] = GetAffineTilePixelColorIdx(
                    tileDataPtr,
                    tileX,
                    tileY
            );
//
//            std::cout << std::dec << "x:" << screenX << " Y:" << screenY
//                      << " Tx:" << textureX << " Ty:" << textureY
//                      << " mx:" << mapX << " my:" << mapY
//                      << " tx:" << tileX << " ty:" << tileY
//                      << " bgx:" << (bgx >> 8) << " bgy:" << (bgy >> 8) ;
//            std::cout << " cidx:" << std::hex << (int)bgLineBuf[ layerNum ][ screenX ] << std::endl ;
        } // for
    } // ppu_DrawTextBackGroundLayer()
}