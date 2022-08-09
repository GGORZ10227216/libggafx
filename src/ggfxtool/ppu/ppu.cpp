//
// Created by orzgg on 2022-01-16.
//

#include "ggfxtool/ppu.h"

#include <algorithm>
#include <iostream>
#include <bit_manipulate.h>
#include <queue>

#include <cstring>
#include <chrono>

#include <thread>

namespace gg_gfx {
    ppu::ppu(gg_raw_data &ioReg, gg_raw_data &palette, gg_raw_data &vram, gg_raw_data &oam) :
            rPalette(palette),
            rVram(vram),
            rOam(oam),
            rIoReg(ioReg) {
//        rIoReg[ MOSAIC - 0x0400'0000 ] = 0x00e2;
        for (int i = 0; i < 4; ++i)
            bgLineBuf[i].resize(248);

        infoBuffer.resize(240);

        frameBuffer.resize(240 * 160);

        rStart = std::chrono::steady_clock::now();
    }

    uint16_t ppu::ReadBgPaletteRaw(int idx) {
        return reinterpret_cast<uint16_t *>(rPalette.data())[idx];
    } // ppu::ReadPalette()

    uint32_t ppu::ReadBgPaletteColor(int idx) {
        uint16_t rawColor = ReadBgPaletteRaw(idx) ;

        uint8_t r, g, b, a;

        r = (rawColor & 0x1f) << 3 ;
        g = (rawColor & 0x3e0) >> 2 ;
        b = (rawColor & 0x7c00) >> 7 ;
        a = 0xff ;

        return r | (g << 8) | (b << 16) | (a << 24) ;
    } // ppu::ReadPaletteColor()

    uint16_t ppu::ReadObjPaletteRaw(int idx) {
        return reinterpret_cast<uint16_t *>(rPalette.data() + 0x200)[idx];
    } // ppu::ReadPalette()

    uint32_t ppu::ReadObjPaletteColor(int idx) {
        uint16_t rawColor = ReadObjPaletteRaw(idx) ;

        uint8_t r, g, b, a;

        r = (rawColor & 0x1f) << 3 ;
        g = (rawColor & 0x3e0) >> 2 ;
        b = (rawColor & 0x7c00) >> 7 ;
        a = 0xff ;

        return r | (g << 8) | (b << 16) | (a << 24) ;
    } // ppu::ReadPaletteColor()

    uint8_t *ppu::GetBgTileDataPtr(BG_TYPE bgType, int tileBlockNum, int layerNum, int tileNum) {
        uint16_t bgcnt = BG_CNT::GetBGCNT(rIoReg.data(), layerNum);
        uint8_t *base = rVram.data() + (tileBlockNum * 0x4000);

        return base + BG_CNT::TileBlockSize(bgcnt, bgType) * tileNum;
    } // GetTileDataPtr()

    void ppu::DrawBGTileLine(int rowNum, int paletteNum, uint8_t *tileData, uint32_t *dstBuffer) {
        constexpr int pixelPerRow = 8;
        const int bytePerRow = (paletteNum == -1) ? 8 : 4;

        if (bytePerRow == 4) {
            uint16_t *palette = reinterpret_cast<uint16_t*>(rPalette.data()) ;
            palette += paletteNum*16 ;

            for (int i = 0, j = 0; i < pixelPerRow; i += 2) {
                uint8_t *tileRow = tileData + rowNum * bytePerRow;
                dstBuffer[i] = gg_utility::RGB555_TO_RGB8888(palette[(tileRow[j] & 0xf)]);
                dstBuffer[i + 1] = gg_utility::RGB555_TO_RGB8888(palette[(tileRow[j] >> 4)]);
                ++j;
            } // for
        } // if
        else {
            uint16_t *palette = reinterpret_cast<uint16_t*>(rPalette.data()) ;

            for (int i = 0; i < pixelPerRow; ++i) {
                uint8_t *tileRow = tileData + rowNum * bytePerRow;
                dstBuffer[i] = gg_utility::RGB555_TO_RGB8888(palette[tileRow[i]]);
            } // for
        } // else 
    } // DrawBGTileLine()

    void ppu::DrawScreenLine(int rowNum) {
        const uint16_t dispcnt = DISP_CNT::GetDISPCNT(rIoReg.data());
        int rLayerStart = 0, rLayerEnd = 0;

        // Step 1: figure out what's the target layers, then render them all in ppu::bgLineBuf.
        switch (DISP_CNT::Mode(dispcnt)) {
            case 0:
                rLayerStart = 0;
                rLayerEnd = 3;
                for (int i = 0; i < 4; ++i)
                    DrawTextBackgroundScanline(i, rowNum);
                break;
            case 1:
                rLayerStart = 0;
                rLayerEnd = 2;
                // TODO: render line, need TRANSFORM_BG drawing func
                DrawTextBackgroundScanline(0, rowNum);
                DrawTextBackgroundScanline(1, rowNum);
                DrawAffineBackgroundScanline(2, rowNum);
                break;
            case 2:
                rLayerStart = 2;
                rLayerEnd = 3;
                DrawAffineBackgroundScanline(2, rowNum);
                DrawAffineBackgroundScanline(3, rowNum);
                // TODO: render line, need TRANSFORM_BG drawing func
                break;
            case 3:
                // TODO: pixel mode
                break;
            case 4:
                // TODO: pixel mode
                break;
            case 5:
                // TODO: pixel mode
                break;
        } // switch

        // Step 2: Check is there any window or pixel SFX in this line?
        const uint16_t bldcnt = BLD_CNT::GetBLDCNT(rIoReg.data());

        bool blendEnable = BLD_CNT::Mode(rIoReg.data()) != BLD_MODE::NONE;
        // Step 3: Pick the right Stage 2 ComposeLine function according to Step 2,
        //         then do the layer sorting and actual compose action in that function.

        // Fill infoBuffer by BACKDROP
        // std::ranges::fill(infoBuffer.begin(), infoBuffer.end(), 0);

        // Fill framebuffer of this row by backdrop first
//        std::ranges::fill(
//                frameBuffer.begin() + 240 * rowNum,
//                frameBuffer.begin() + 240 * (rowNum + 1),
//                ReadBgPaletteRaw(0)
//        );

//        int winStatus = gg_core::BitFieldValue<13, 2>(dispcnt);
//        if (winStatus != 0)
//            DetermineWindowStatus(winStatus, rowNum);

        GenerateZBuffer<true>(rowNum, rLayerStart, rLayerEnd);

        for (int xCur = 0 ; xCur < 240 ; ++xCur) {
            switch (infoBuffer[xCur].sfx) {
                case BLD_MODE::BRIGHTEN:
                    frameBuffer[ 240*rowNum + xCur ] = Brighten(infoBuffer[xCur]) ;
                    break ;
                case BLD_MODE::DARKEN:
                    frameBuffer[ 240*rowNum + xCur ] = Darken(infoBuffer[xCur]) ;
                    break ;
                default:
                    frameBuffer[ 240*rowNum + xCur ] = ReadBgPaletteRaw(infoBuffer[xCur].colorIdx & 0xff);
            } // switch
        } // for

//        exit(0);
    } // DrawScreenLine()

    void ppu::DetermineWindowStatus(int &winStatus, const int rowNum) {
        // FIXME: need refactor
        for (int winIdx = 1; winIdx >= 0; --winIdx) {
            if (gg_core::TestBit(winStatus, winIdx)) {
                WinRect winRect = WINDOW::GetWindowRect(rIoReg.data(), winIdx);
                int start = winRect.x1;
                int end = winRect.x2;

                bool rowInWin = (winRect.y2 >= winRect.y1) ?
                                (rowNum >= winRect.y1 && rowNum < winRect.y2) : (rowNum >= winRect.y1 ||
                                                                                 rowNum < winRect.y2);

                if (!rowInWin) {
                    gg_core::ClearBit(winStatus, winIdx);
                    continue;
                } // if
                else {
                    std::ranges::fill(winZ_Buffer.begin(), winZ_Buffer.end(), OUT);
                    while (start != end) {
                        winZ_Buffer[start] = WIN0 + winIdx;
                        start = (start + 1) % 240;
                    } // while
                } // else
            } // if
        } // for
    } // DetermineWindowStatus

    uint8_t* ppu::GetSpriteTile_1D(const ObjAttr& attr, int spriteX, int spriteY) {
        enum { SPRITE_MULTIPLIER = 0x20 } ;

        const int spriteName = attr.characterName() ;

        const uint32_t objTileAreaOffset = [&]() {
            const int dispcnt = DISP_CNT::GetDISPCNT(rIoReg.data());
            return DISP_CNT::Mode(dispcnt) < 3 ? 0x10000 : 0x14000 ;
        }();

        const ObjRect rect = attr.objRect() ;
        const int bytesPerTile = attr.pixelSize() == NIBBLE ? 0x20 : 0x40 ;
        const int tilesPerRow = rect.w / 8 ;

        const uint32_t inSpriteOffset = bytesPerTile * tilesPerRow * (spriteY / 8) +
                bytesPerTile * (spriteX / 8) ;

        return (rVram.data() + objTileAreaOffset + spriteName * SPRITE_MULTIPLIER + inSpriteOffset);
    } // GetSpriteCharacterAdd_1D()

    uint8_t* ppu::GetSpriteTile_2D(const ObjAttr& attr, int spriteX, int spriteY) {
        enum { SPRITE_MULTIPLIER = 0x20 } ;

        const int spriteName = attr.characterName() ;

        const uint32_t objTileAreaOffset = [&]() {
            const int dispcnt = DISP_CNT::GetDISPCNT(rIoReg.data());
            return DISP_CNT::Mode(dispcnt) < 3 ? 0x10000 : 0x14000 ;
        }();

//        const ObjRect rect = attr.objRect() ;
        uint8_t* tileData =
                (rVram.data() + objTileAreaOffset) +
                (spriteName * SPRITE_MULTIPLIER) +
                ((spriteY / 8) * 0x400) +
                ((spriteX / 8) * 0x20) ;

        return tileData ;
    } // GetSpriteCharacterAdd_2D()

    uint16_t ppu::GetTilePixelColor(PIXEL_SIZE pixelSize, int tileNum, int tileX, int tileY, int paletteNum, bool isObj) {
        const int tileSize = pixelSize == NIBBLE ? 0x20 : 0x40 ;
        enum { FOUR_BPP_TILE_LINE_SIZE = 0x4, EIGHT_BPP_TILE_LINE_SIZE = 0x8 };

        int baseAddr = 0x0 ;
        int colorIdx = 0 ;

        if (isObj) {
            baseAddr = [&]() {
                const int dispcnt = DISP_CNT::GetDISPCNT(rIoReg.data());
                return DISP_CNT::Mode(dispcnt) < 3 ? 0x10000 : 0x14000 ;
            }();
        } // if

        if (pixelSize == NIBBLE) {
            const int inTileOffset = FOUR_BPP_TILE_LINE_SIZE * tileY + (tileX / 2) ; // 1 byte for 2 pixel
            const uint8_t *const pixelDataPtr = rVram.data() + baseAddr + tileSize * tileNum + inTileOffset ;

            if (tileX % 2) // odd pixel
                colorIdx = (*pixelDataPtr) >> 4 ;
            else // even pixel
                colorIdx = (*pixelDataPtr) & 0xf ;

            colorIdx += paletteNum*16 ;
        } // if
        else {
            const int inTileOffset = EIGHT_BPP_TILE_LINE_SIZE * tileY + tileX ;
            const uint8_t *const pixelDataPtr = rVram.data() + baseAddr + tileSize * tileNum + inTileOffset ;

            colorIdx = *pixelDataPtr ;
        } // else

        return isObj ? ReadBgPaletteRaw(colorIdx) : ReadObjPaletteRaw(colorIdx) ;
    } // ppu::GetTileData()

//    void ppu::RawColor_DrawTile(uint16_t* plotPtr, PIXEL_SIZE pixelSize, int tileNum, int paletteNum, bool isObj) {
//        enum { FOUR_BPP_PALETTE_LENGTH = 16 } ;
//        for (int tileY = 0 ; tileY < 8 ; ++tileY) {
//            for (int tileX = 0 ; tileX < 8 ; ++tileX) {
//                int pixelColorIdx = GetTilePixelData(pixelSize, tileNum, tileX, tileY) ;
//                if (pixelSize == NIBBLE)
//                    pixelColorIdx += paletteNum * FOUR_BPP_PALETTE_LENGTH ;
//
//                if (isObj)
//                    *plotPtr = ReadObjPaletteRaw(pixelColorIdx) ;
//                else
//                    *plotPtr = ReadBgPaletteRaw(pixelColorIdx) ;
//            } // for
//        } // for
//    } // ppu::RawColor_DrawTile()
}