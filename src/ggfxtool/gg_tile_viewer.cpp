//
// Created by orzgg on 2022-01-10.
//

#include <iostream>

#include <gg_tile_viewer.h>

#include <glad/glad.h>

namespace gg_widget
{
    gg_tile_viewer::gg_tile_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum) :
        gg_abstract_widget(parent, "Tile", 240, 320, serialNum),
        rVramRaw(pPpu->rVram),
        _pPpu(pPpu)
    {
        _tileMapTex = std::make_unique<gg_gfx::gg_texture>(parent, 256, 256);

        DrawTiles();
    }

    void gg_tile_viewer::DrawTiles()
    {
        const int tileSize = paletteNumber == -1 ? 0x40 : 0x20;
        const int tilePerRow = paletteNumber == -1 ? 16 : 32;

        const int totalTileNumberInBlock = 0x4000 / tileSize;

        uint8_t *tileBlockBaseAddr = rVramRaw.data() + dumpStartAddr;

        // Clear texture's data first
        for (int i = 0; i < 256 * 256; ++i)
            ((uint32_t *)_tileMapTex->GetRawData())[i] = 0xff000000;

        // Render all tiles in current block
        for (int i = 0x0; i < totalTileNumberInBlock; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                _pPpu->DrawBGTileLine(
                    j,
                    paletteNumber,
                    tileBlockBaseAddr + tileSize * i,
                    (uint32_t *)_tileMapTex->GetRawDataXY(8 * (i % tilePerRow), 8 * (i / tilePerRow) + j));
            } // for
        }     // for

        _tileMapTex->Update();
    } // DrawTiles()

    void gg_tile_viewer::OnPaletteChange()
    {
        ImGui::BeginChild("PaletteSettingWindow", ImVec2(ImGui::GetContentRegionAvail().x, 256), ImGuiWindowFlags_NoScrollbar);

        ImGui::BulletText("Viewing Area:", ImGui::GetContentRegionAvail().x);
        ImGui::Indent();
        if (ImGui::BeginCombo("##TileArea", currentDisplayArea, 0))
        {
            for (int i = 0; i < IM_ARRAYSIZE(areaAddress); ++i)
            {
                const bool is_selected = (currentDisplayAreaIdx == i);
                if (ImGui::Selectable(areaAddress[i], is_selected))
                {
                    currentDisplayArea = areaAddress[i];
                    dumpStartAddr = strtol(currentDisplayArea, NULL, 16) - 0x0600'0000;
                    currentDisplayAreaIdx = i;
                    DrawTiles();
                } // if

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            } // for

            ImGui::EndCombo();
        } // if
        ImGui::Unindent();

        ImGui::BulletText("Color depth", ImGui::GetContentRegionAvail().x);
        ImGui::Indent();
        ImGui::RadioButton("4 bpp", &paletteMode, 0);
        ImGui::SameLine();
        ImGui::RadioButton("8 bpp", &paletteMode, 1);
        ImGui::Unindent();

        if (paletteMode == 1)
        {
            ImGui::BeginDisabled();
            fullTileDumpTextureWidth = 256;
            paletteNumber = -1;
        } // if
        else
        {
            paletteNumber = lastPaletteNumber;
            fullTileDumpTextureWidth = 512;
        } // else

        ImGui::BulletText("Palette: ", ImGui::GetContentRegionAvail().x);
        ImGui::Indent();
        if (ImGui::BeginCombo("##Palette select", currentPalette, 0))
        {
            for (int n = 0; n < IM_ARRAYSIZE(paletteNames); n++)
            {
                const bool is_selected = (paletteNumber == n);
                if (ImGui::Selectable(paletteNames[n], is_selected))
                {
                    currentPalette = paletteNames[n];
                    paletteNumber = n;
                } // if

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        } // if
        ImGui::Unindent();

        if (paletteMode == 1)
            ImGui::EndDisabled();
        ImGui::EndChild();

        if (paletteMode != lastPaletteMode || lastPaletteNumber != paletteNumber)
        {
            if (paletteNumber != -1)
                lastPaletteNumber = paletteNumber;
            lastPaletteMode = paletteMode;
            DrawTiles();
        } // if
    }     // OnPaletteChange()

    void gg_tile_viewer::Draw()
    {
        Sizes s;
        CalcSizes(s, rVramRaw.size(), 0x0);
        ImGui::SetNextWindowSize(ImVec2(s.WindowWidth, s.WindowWidth * 0.60f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 0.0f), ImVec2(s.WindowWidth, FLT_MAX));

        if (ImGui::Begin(name.c_str(), &alive, ImGuiWindowFlags_NoScrollbar))
        {
            if (ImGui::BeginTabBar("ViewSwitch", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("HexView"))
                {
                    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                        ImGui::OpenPopup("context");
                    DrawContents(rVramRaw.data(), rVramRaw.size(), 0x0);
                    if (ContentsWidthChanged)
                    {
                        CalcSizes(s, rVramRaw.size(), 0x0);
                        ImGui::SetWindowSize(ImVec2(s.WindowWidth, ImGui::GetWindowSize().y));
                    }

                    ImGui::EndTabItem();
                } // if

                if (ImGui::BeginTabItem("Tile View"))
                {
                    ImGui::BeginChild("TileTexture", ImVec2(fullTileDumpTextureWidth, fullTileDumpTextureHeight), false, ImGuiWindowFlags_NoScrollbar);
                    ImGui::Image(_tileMapTex->GetTextureId(), ImVec2(512, 512));
                    ImGui::EndChild();

                    OnPaletteChange();

                    ImGui::EndTabItem();
                } // if
            }     // if

            ImGui::EndTabBar();
        }

        ImGui::End();
    } // Draw()
} // gg_widget