//
// Created by orzgg on 2022-01-16.
//

#include <array>
#include <tuple>

#include <gg_type.h>
#include <io_enum.h>

#include <bgcnt.h>
#include <mosaic.h>
#include <bit_manipulate.h>
#include "ggfxtool/gg_gfx_utility.h"
#include "gg_texture.h"
#include <oam.h>

#include <ppu_enum.h>
#include <dispcnt.h>
#include <bldcnt.h>
#include <window.h>
#include <chrono>
#include <cstring>

#ifndef LIBGAFX_PPU_H
#define LIBGAFX_PPU_H

namespace gg_gfx {
    class ppu {
    public :
        ppu() = delete;

        ppu(gg_raw_data &ioReg, gg_raw_data &palette, gg_raw_data &vram, gg_raw_data &oam);

        uint16_t ReadBgPaletteRaw(int idx);

        uint32_t ReadBgPaletteColor(int idx);

        uint16_t ReadObjPaletteRaw(int idx);

        uint32_t ReadObjPaletteColor(int idx);

        void WriteBgPaletteRaw(int idx, uint16_t color555);

        void WriteObjPaletteRaw(int idx, uint16_t color555);

        int GetMapBlockNumber(int x, int y, int bgWidth) {
            const int numOfBlkPerRow = bgWidth / 8;
            const int xBlkAxis = x / 8;
            const int yBlkAxis = y / 8;

            return yBlkAxis * numOfBlkPerRow + xBlkAxis;
        } // GetMapBlockNumber()

        uint8_t *GetBgTileDataPtr(BG_TYPE bgType, int tileBlockNum, int layerNum, int tileNum);

        uint8_t *GetBgTileDataPtrXY(BG_TYPE bgType, int tileBlockNum, int layerNum, int tileNum);

        static uint64_t inline
        Get4bppTilePixelColorIdx(uint8_t const *tilePtr, int y, int p, bool xInverse, bool yInverse) {
            if (yInverse) {
                y = 7 - y;
            } // if

            uint64_t result = 0;
            uint64_t buf = *reinterpret_cast<uint32_t const *>(tilePtr + y * 4);
            uint64_t wp = p << 4; // *16

            int mask = 0xf;

            if (xInverse) {
                // Swap the nibbles of each byte.
                buf = (buf & 0x0F0F0F0F) << 4
                      | (buf & 0xF0F0F0F0) >> 4;

                // Swap the bytes of each byte pair.
                buf = (buf & 0x00FF00FF) << 8
                      | (buf & 0xFF00FF00) >> 8;

                // Swap the byte pairs.
                buf = (buf & 0x0000FFFF) << 16
                      | (buf & 0xFFFF0000) >> 16;
            } // if

            for (int i = 0; i < 8; ++i) {
                result |= ((buf & mask) << (i << 2)) | wp;
                wp <<= 8;
                mask <<= 4;
            } // for

            return result;
        } // Get4bppTilePixelDataPtr()

        static uint64_t Get8bppTilePixelColorIdx(uint8_t const *tilePtr, int y, bool xInverse, bool yInverse) {
            if (yInverse) {
                y = 7 - y;
            } // if

            uint64_t buf = *reinterpret_cast<uint32_t const *>(tilePtr + y * 8);
            if (xInverse) {
                // Swap the bytes of each byte pair.
                buf = (buf & 0x00FF00FF) << 8
                      | (buf & 0xFF00FF00) >> 8;

                // Swap the byte pairs.
                buf = (buf & 0x0000FFFF) << 16
                      | (buf & 0xFFFF0000) >> 16;
            } // if

            return buf;
        } // Get8bppTilePixelColorIdx()

        static uint8_t GetAffineTilePixelColorIdx(uint8_t const *tilePtr, int x, int y) {
            assert(("Invalid tile X value", x >= 0 && x < 8)) ;
            assert(("Invalid tile Y value", y >= 0 && y < 8)) ;
            return *(tilePtr + (y << 3) + x)  ;
        } // GetAffineTilePixelColorIdx()

        void DrawBGTileLine(int rowNum, int paletteNum, uint8_t *tileData, uint32_t *dstBuffer);

        void DrawTextBackgroundScanline(int layerNum, int rowNum);
        void DrawAffineBackgroundScanline(int layerNum, int rowNum);
        void DrawSpriteLine(uint8_t* vBuf, int spriteY, ObjAttr* attr) ;

        /* Debugger API */
        uint16_t GetTilePixelColor(PIXEL_SIZE pixelSize, int tileNum, int tileX, int tileY, int paletteNum, bool isObj) ;
        /* Debugger API */
        void DrawTextBGScanline(int rowNum);

        void DrawScreenLine(int rowNum);
//        void ComposeBGScanline(int startLayer, int endLayer, int rowNum, int winEnable) ;

//        inline void DrawNormalScanline(int layerNum, int rowNum) {
//            uint16_t HOFS = (rIoReg[ BG0HOFS - 0x0400'0000 + 0x4*layerNum ] & 0xf) % 8 ;
//
//            for (int i = 0 ; i < 240 ; i+=4) {
//                int realCursorX = ((i + HOFS) % 256) ;
//                uint32_t colorIndex = reinterpret_cast<uint32_t&>(bgLineBuf[layerNum][realCursorX]) ;
//                uint8_t* cptr = (uint8_t*)&colorIndex ;
//
//                uint16_t rawColors[ 4 ];
//                for (int k = 0 ; k < 4 ; ++k) {
//                    if ((cptr[k] % 16) == 0) {
//                        rawColors[ k ] = frameBuffer[240 * rowNum + i + k] ;
//                    } // if
//                    else {
//                        rawColors[ k ] = ReadBgPaletteRaw(cptr[k]) ;
//                        infoBuffer[i + k] = layerNum + 1;
//                    } // else
//                } // for
//
//                for (int k = 0 ; k < 4 ; ++k)
//                    frameBuffer[240 * rowNum + i + k] = rawColors[ k ];
//            } // for
//        }

//        void AlphaBlending(BLD_MODE bldMode, int layerNum, int xPos, int yPos) {
//            uint8_t colorIndex = bgLineBuf[layerNum][ xPos ] ;
//            if (colorIndex % 16 != 0) {
//                switch (bldMode) {
//                    case BLD_MODE::NORMAL:
//                    {
//                        const int firstTarget = BLD_CNT::FirstTarget(rIoReg.data());
//                        const int secondTarget = BLD_CNT::SecondTarget(rIoReg.data());
//                        const bool layerIsFirstTarget = firstTarget & gg_core::_BV(layerNum) ;
//                        const bool isValidBlend = secondTarget & infoBuffer[ xPos ] ;
//
////                        if (layerIsFirstTarget && isValidBlend) {
////                            int
////                        } // if
//                    } break;
//                    case BLD_MODE::BRIGHTEN:
//                        break;
//                    case BLD_MODE::DARKEN: {
//                        const int firstTarget = BLD_CNT::FirstTarget(rIoReg.data());
//                        const bool layerIsFirstTarget = firstTarget & gg_core::_BV(layerNum) ;
//                        if (layerIsFirstTarget) {
//                            uint16_t rawColor = ReadBgPaletteRaw(colorIndex), result ;
//                            int EVY = BLD_CNT::EVY(rIoReg.data());
//
//                            int r = rawColor & 0x1f ;
//                            int g = (rawColor >> 5) & 0x1f ;
//                            int b = (rawColor >> 10) & 0x1f ;
//
//                            uint32_t buf = r | g << 8 | b << 16 ;
//
//                            buf = (buf*EVY) >> 4 ;
//
//                            r = buf & 0xf ;
//                            g = (buf & 0xf00) >> 8 ;
//                            b = (buf & 0xf0000) >> 16 ;
//
//                            result = (b << 10) | (g << 5) | r ;
//
//                            frameBuffer[ 240*yPos + xPos ] = result;
//                        } // if
//                        else
//                            frameBuffer[ 240*yPos + xPos ] = ReadBgPaletteRaw(colorIndex);
//                    } break;
//                } // switch
//
//                infoBuffer[ xPos ] = layerNum ;
//            } // if
//        } // AlphaBlending()

        struct RENDER_INFO {
            enum {
                BD, BG0, BG1, BG2, BG3, OBJ
            };
            uint16_t colorIdx = 0; // (MSB) second : top (LSB)
            uint16_t z = 0;
            BLD_MODE sfx = BLD_MODE::NONE;

            void SetTopLayer(int first, int zIdx) {
                gg_core::rotr(colorIdx, 8);
                colorIdx = (colorIdx & 0xff00) | first;
                gg_core::rotr(z, 8);
                z = (z & 0xff00) | zIdx;
            } // SetFirst()
        };

        struct WIN_INFO {
            int idx, enabledLayer ;
            BLD_MODE sfx ;
        };

        int Darken(RENDER_INFO info) {
            int firstTarget = BLD_CNT::FirstTarget(rIoReg.data());
            int firstZ = info.z & 0xff;

            if (gg_core::TestBit(firstTarget, firstZ)) {
                uint16_t rawColor = ReadBgPaletteRaw(info.colorIdx & 0xff);
                int EVY = BLD_CNT::EVY(rIoReg.data());

                int r = rawColor & 0x1f;
                int g = (rawColor >> 5) & 0x1f;
                int b = (rawColor >> 10) & 0x1f;

//                uint32_t buf = r | g << 8 | b << 16;

//                buf = (buf * EVY) >> 4;

                r = (r * EVY) >> 4 ;
                g = (g * EVY) >> 4 ;
                b = (b * EVY) >> 4 ;

//                r = buf & 0xf;
//                g = (buf & 0xf00) >> 8;
//                b = (buf & 0xf0000) >> 16;

                return (b << 10) | (g << 5) | r;
            } // if
            else
                return ReadBgPaletteRaw(info.colorIdx & 0xff);
        } // Darken()

        int Brighten(RENDER_INFO info) {
            int firstTarget = BLD_CNT::FirstTarget(rIoReg.data());
            int firstZ = info.z & 0xff;

            if (gg_core::TestBit(firstTarget, firstZ)) {
                uint16_t rawColor = ReadBgPaletteRaw(info.colorIdx & 0xff);
                int EVY = BLD_CNT::EVY(rIoReg.data());

                int r = rawColor & 0x1f;
                int g = (rawColor >> 5) & 0x1f;
                int b = (rawColor >> 10) & 0x1f;

                r += ((31 - r)*EVY) >> 4;
                g += ((31 - g)*EVY) >> 4;
                b += ((31 - b)*EVY) >> 4;

                return (b << 10) | (g << 5) | r;
            } // if
            else
                return ReadBgPaletteRaw(info.colorIdx & 0xff);
        } // Brighten()

        uint32_t (*PixelColorCorrectionFunc)(uint16_t) = gg_utility::RGB555_TO_RGB8888;

        gg_raw_data bgLineBuf[4];

        gg_raw_data &rIoReg;
        OAM rOam ;
        gg_raw_data &rPalette;
        gg_raw_data &rVram;

        std::vector<RENDER_INFO> infoBuffer;

        gg_raw_data winZ_Buffer;
        std::vector<uint16_t> frameBuffer;

        std::vector<uint32_t> trueColorBgPalette;
        std::vector<uint32_t> trueColorObjPalette;

        uint64_t fcnt = 0;
        std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<int64_t, std::ratio<1, 1000000000>>> rStart, rLast;

    private :
        int mosaicVCount = 0;
        bool log = false;

        int enabledWinNum = 0, bgLineSize = 0, bgLineOffset = 0;
//        std::array<WIN_INFO, 2> enabledWin;

        void DetermineWindowStatus(int &winStatus, const int rowNum);

        uint8_t* GetSpriteTile_1D(const ObjAttr& attr, int spriteX, int spriteY) ;
        uint8_t* GetSpriteTile_2D(const ObjAttr& attr, int spriteX, int spriteY) ;

        bool TestWinX(WinRect winRect, int x) {
            const bool xAxisChk = (x >= winRect.x1 && x < winRect.x2) ||
                                  (x >= winRect.x2 && x < (240 - winRect.x1 + winRect.x2) % 240);

            if (xAxisChk)
                return true;
            else
                return false;
        } // TestWin()

        bool TestWinY(WinRect winRect, int y) {
            int y1 = winRect.y1, y2 = winRect.y2 ;

            const bool isLooping = y1 > y2 ;
            const bool isBetween = y >= std::min(y1, y2) && y < std::max(y1, y2) ;

            if (!isLooping && isBetween)
                return true ;
            else if (isLooping && !isBetween)
                return true ;
            else
                return false ;
        } // TestWin()

        template<bool WIN_ENABLE>
        void GenerateZBuffer(int rowNum, int startLayer, int endLayer) {
            const uint16_t dispcnt = DISP_CNT::GetDISPCNT(rIoReg.data());
            const uint8_t bgWinStatus = gg_core::BitFieldValue<13, 2>(dispcnt);

            std::array<int, 4> layerQueue;
            layerQueue.fill(-1);

            for (int p = 3, idx = 0; p >= 0; --p) {
                for (int targetLayer = endLayer; targetLayer >= startLayer; --targetLayer) {
                    uint16_t bgcnt = BG_CNT::GetBGCNT(rIoReg.data(), targetLayer);
                    if (BG_CNT::Priority(bgcnt) == p) {
                        layerQueue[idx++] = targetLayer;
                    } // if
                } // for
            } // for

            memset(infoBuffer.data(), 0, 240 * sizeof(RENDER_INFO));

            BLD_MODE bldMode = BLD_CNT::Mode(rIoReg.data()) ;

            std::array<char,240> winZ ;
            std::array<WinRect, 2> winRects ;

            if constexpr (WIN_ENABLE) {
                memset(winZ.data(), 0 ,240);

                winRects = {
                        WINDOW::GetWindowRect(rIoReg.data(), 0),
                        WINDOW::GetWindowRect(rIoReg.data(), 1)
                };

                int winStatus = gg_core::BitFieldValue<13, 2>(dispcnt);

                for (int winIdx = 1; winIdx >= 0; --winIdx) {
                    bool winEnabled = gg_core::TestBit(winStatus, winIdx);

                    bool rowInWin = TestWinY(winRects[ winIdx ], rowNum);
                    if (winEnabled && rowInWin) {
//                        bool winSfxEnable = WINDOW::WININ_SFX_ENABLED(rIoReg.data(), winIdx) ;
//                        enabledWin[enabledWinNum].idx = winIdx ;
//                        enabledWin[enabledWinNum].sfx = winSfxEnable ? bldMode : BLD_MODE::NONE ;
//                        enabledWin[enabledWinNum].enabledLayer = ((uint8_t*)&winIn)[winIdx] ;
                        int x1 = winRects[ winIdx ].x1, x2 = winRects[ winIdx ].x2 ;
                        int winWidth = x2 > x1 ? x2 - x1 : 240 - x1 + x2 ;
                        for (int wx = winRects[ winIdx ].x1, p = 0 ; p < winWidth ; ++p) {
                            winZ[ wx ] = winIdx + 1 ;
                            wx = (wx + 1) % 240 ;
                        } // for
//                        ++enabledWinNum;
                    } // if
                } // for
            } // if

            int j = 0;
            while (j < layerQueue.size() && layerQueue[j] != -1) {
                const int layerNum = layerQueue[j];

                bool layerEnabled = DISP_CNT::BgEnabled(dispcnt, layerNum);
                if (layerEnabled) {
                    if constexpr (WIN_ENABLE) {
                        enabledWinNum = 0;
                        const bool layerInWINOUT = WINDOW::WINOUT_LAYER_ENABLED(rIoReg.data(), layerNum) ;
                        const bool winOutSfxEnable = WINDOW::WINOUT_SFX_ENABLED(rIoReg.data());
                        BLD_MODE winOutSfx = winOutSfxEnable ? bldMode : BLD_MODE::NONE ;

                        for (int i = 0 ; i < 240 ; i+=4) {
                            int realCursorX = ((i + bgLineOffset) % bgLineSize);
                            uint32_t colorIdx = reinterpret_cast<uint32_t&>(bgLineBuf[layerNum][realCursorX]);
                            uint8_t* cptr = (uint8_t*)&colorIdx ;
                            const uint16_t bgcnt = BG_CNT::GetBGCNT(rIoReg.data(), layerNum) ;
                            auto isBackdropPixel = [&](int k) -> bool {
                                const bool isAffineModeBG = layerNum >=2 &&
                                        DISP_CNT::Mode(DISP_CNT::GetDISPCNT(rIoReg.data())) > 0 ;
                                if ((BG_CNT::PixelSize(bgcnt) == PIXEL_SIZE::NIBBLE) && !isAffineModeBG)
                                    return cptr[k] % 16 == 0 ;
                                else
                                    return cptr[k] == 0 ;
                            };

                            for (int k = 0 ; k < 4 ; ++k) {
                                if (!isBackdropPixel(k)) {
                                    if (winZ[ i + k ] != 0 && WINDOW::WIN_LAYER_ENABLED(rIoReg.data(), winZ[ i + k ] - 1, layerNum)) {
                                        int winIdx = winZ[ i + k ] - 1 ;
                                        bool winSfxEnable = WINDOW::WININ_SFX_ENABLED(rIoReg.data(), winIdx) ;
                                        infoBuffer[ i + k ].SetTopLayer(cptr[ k ], layerNum) ;
                                        infoBuffer[ i + k ].sfx = winSfxEnable ? bldMode : BLD_MODE::NONE ;
                                    } // if
                                    else if (layerInWINOUT && winZ[ i + k ] == 0) {
                                        infoBuffer[ i + k ].SetTopLayer(cptr[ k ], layerNum) ;
                                        infoBuffer[ i + k ].sfx = winOutSfx ;
                                    } // if
                                    else  {
                                        // This layer is both outside of winIn and winOut
                                    } // else
                                } // if
                            } // for
                        } // for
                    } // if
                    else {
                        for (int i = 0; i < 240; i += 4) {
                            int realCursorX = ((i + bgLineOffset) % bgLineSize);
                            uint32_t colorIndex = reinterpret_cast<uint32_t &>(bgLineBuf[layerNum][realCursorX]);
                            uint8_t *cptr = (uint8_t *) &colorIndex;

                            for (int k = 0; k < 4; ++k) {
                                if ((cptr[k] % 16) != 0) {
                                    infoBuffer[i + k].SetTopLayer(cptr[k], layerNum);
                                } // if
                            } // for
                        } // for
                    } // else
                } // if

                ++j;
            } // while
        } // GenerateZBuffer()
    };
}

#endif //LIBGAFX_PPU_H
