//
// Created by orzgg on 2022-01-10.
//

#include <memory>
#include <functional>

#include <imgui.h>
#include <imgui_memory_editor.h>

#include <gg_type.h>
#include <gg_abstract_widget.h>
#include <gg_texture.h>

#include <ppu.h>

#ifndef LIBGAFX_GG_TILE_VIEWER_H
#define LIBGAFX_GG_TILE_VIEWER_H

namespace gg_widget
{
    class gg_tile_viewer : public gg_abstract_widget, public MemoryEditor
    {
    public:
        gg_tile_viewer() = delete;
        gg_tile_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum);

        void Draw() override;

        gg_raw_data rVramRaw;

        uint32_t dumpStartAddr = 0x0;
        int paletteNumber = 0, lastPaletteNumber = 0;

        void DrawTiles();

        std::unique_ptr<gg_gfx::gg_texture> _tileMapTex;
        std::shared_ptr<gg_gfx::ppu> _pPpu;

    private:
        void OnPaletteChange();
        int paletteMode = 0, lastPaletteMode = 0; // 0 --> 4bpp, 1 --> 8bbp
        int currentDisplayAreaIdx = 0 ;

        int fullTileDumpTextureWidth = 512, fullTileDumpTextureHeight = 256 ;

        const char *paletteNames[16] = {
            "#0",
            "#1",
            "#2",
            "#3",
            "#4",
            "#5",
            "#6",
            "#7",
            "#8",
            "#9",
            "#A",
            "#B",
            "#C",
            "#D",
            "#E",
            "#F"
        };

        const char *areaAddress[4] = {
            "0x6000000",
            "0x6004000",
            "0x6008000",
            "0x600c000"
        };

        const char *currentPalette = paletteNames[0];
        const char *currentDisplayArea = areaAddress[0];
    };
} // gg_widget

#endif //LIBGAFX_GG_TILE_VIEWER_H
