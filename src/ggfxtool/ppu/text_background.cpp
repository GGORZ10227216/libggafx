//
// Created by orzgg on 2022/2/18.
//

#include <ppu.h>
#include <bit_manipulate.h>

#include <utility>

#include <chrono>

namespace gg_gfx {
    namespace TextMode {
        static constexpr std::array<std::pair<int, int>, 4> textModeBgSize = {
                std::make_pair(256, 256),
                std::make_pair(512, 256),
                std::make_pair(256, 512),
                std::make_pair(512, 512)
        };
    } // TextMode

    void ppu::DrawTextBackgroundScanline(int layerNum, int rowNum) {
        const int bgcntAddr = BG0CNT - 0x0400'0000 + (layerNum << 1);
        const int hofsAddr = BG0HOFS - 0x0400'0000 + (layerNum << 2);
        const int vofsAddr = BG0VOFS - 0x0400'0000 + (layerNum << 2);

        const uint16_t bgcnt = (uint16_t &) rIoReg[bgcntAddr];
        const int HOFS = rIoReg[hofsAddr]; // initialize by value of HOFS, only LSB 8bit readable
        const int VOFS = rIoReg[vofsAddr]; // initialize by value of VOFS, only LSB 8bit readable
        const uint16_t mosaic_H = MOSAIC_FX::BG_H_Size(rIoReg[MOSAIC - 0x0400'0000]);
        const uint16_t mosaic_V = MOSAIC_FX::BG_V_Size(rIoReg[MOSAIC - 0x0400'0000]);

        const int fullBgWidth = TextMode::textModeBgSize[BG_CNT::ScreenSizeID(bgcnt)].first;
        const int fullBgHeight = TextMode::textModeBgSize[BG_CNT::ScreenSizeID(bgcnt)].second;

        const int mapBaseBlkNumber = BG_CNT::MapBaseBlockNumber(bgcnt);
        const int tileBaseBlkNumber = BG_CNT::TileBaseBlockNumber(bgcnt);

        int tileBlockSize = BG_CNT::PixelSize(bgcnt) == PIXEL_SIZE::NIBBLE ? 0x20 : 0x40;

        uint16_t const *mapAddrBase = (uint16_t *) (rVram.data() + 0x800 * mapBaseBlkNumber);

        int scrCursorX = 0;

        int bgCursorY = (VOFS + rowNum) % fullBgHeight;

        uint8_t const *tileAddrBase = rVram.data() + tileBaseBlkNumber * 0x4000;

        const int mapIdxBase = (fullBgWidth >> 3) * (bgCursorY >> 3);

        bgLineSize = (HOFS % 8) ? 248 : 240 ;
        bgLineOffset = HOFS % 8 ;
        while (scrCursorX < bgLineSize) {
            int bgCursorX = (HOFS + scrCursorX) % fullBgWidth;

            const int currentMapBlockNum = mapIdxBase + (bgCursorX >> 3);
            int mapData = *(mapAddrBase + currentMapBlockNum);

            int paletteNum = tileBlockSize == 0x40 ? 0 : (mapData & 0xf000) >> 12; // ignored if 8bpp mode
            int tileNum = mapData & 0x3ff;

            uint8_t const *tileDataPtr = tileAddrBase + tileNum * tileBlockSize;

            const uint64_t colorIdx = [&]() {
                if (tileBlockSize == 0x20)
                    return Get4bppTilePixelColorIdx(
                        tileDataPtr,
                        bgCursorY % 8,
                        paletteNum,
                        gg_core::TestBit(mapData, 10),
                        gg_core::TestBit(mapData, 11)
                    );
                else
                    return Get8bppTilePixelColorIdx(
                            tileDataPtr,
                            bgCursorY % 8,
                            gg_core::TestBit(mapData, 10),
                            gg_core::TestBit(mapData, 11)
                    );
            }();


            reinterpret_cast<uint64_t &>(bgLineBuf[layerNum][scrCursorX]) = colorIdx;
            scrCursorX += 8;
        } // while
    } // ppu_DrawTextBackGroundLayer()
}