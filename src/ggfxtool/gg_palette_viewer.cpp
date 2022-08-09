//
// Created by orzgg on 2022-01-10.
//

#include <iostream>

#include <user_interface.h>

#include <gg_palette_viewer.h>

#include <glad/glad.h>

namespace gg_widget {
    gg_palette_viewer::gg_palette_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum) :
        gg_abstract_widget(parent, "Palette", 240, 320, serialNum),
        _rPalette(pPpu->rPalette)
    {
        // TODO: if we change rPaletteRaw, we should update the content of paletteTex too
        gg_raw_data bgTexRaw, objTexRaw ;
        size_t areaSize = 0x100 ; // RGB555(2 bytes) --> RGB888(4 bytes)
        for (int i = 0 ; i < areaSize ; ++i) {
            const uint32_t fullColorBg = pPpu->ReadBgPaletteColor(i) ;
            const uint32_t fullColorObj = pPpu->ReadObjPaletteColor(i) ;

            uint8_t r, g, b ;

            r = (fullColorBg & 0xff);
            g = (fullColorBg & 0xff00) >> 8;
            b = (fullColorBg & 0xff0000) >> 16;

            bgTexRaw.push_back(b) ;
            bgTexRaw.push_back(g) ;
            bgTexRaw.push_back(r) ;
            bgTexRaw.push_back(0xff) ;

            r = (fullColorObj & 0xff);
            g = (fullColorObj & 0xff00) >> 8;
            b = (fullColorObj & 0xff0000) >> 16;

            objTexRaw.push_back(b) ;
            objTexRaw.push_back(g) ;
            objTexRaw.push_back(r) ;
            objTexRaw.push_back(0xff) ;
        } // for

        _bgPaletteTex = std::make_unique<gg_gfx::gg_texture>(
            _parent, 16, 16, std::move(bgTexRaw)
        );

        _objPaletteTex = std::make_unique<gg_gfx::gg_texture>(
            _parent, 16, 16, std::move(objTexRaw)
        );
    }

    void gg_palette_viewer::Draw() {
        Sizes s;
        CalcSizes(s, _rPalette.size(), 0x0);
        ImGui::SetNextWindowSize(ImVec2(s.WindowWidth, s.WindowWidth * 0.60f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, 0.0f), ImVec2(s.WindowWidth, FLT_MAX));

        if (ImGui::Begin(name.c_str(), &alive, ImGuiWindowFlags_NoScrollbar))
        {
            if (ImGui::BeginTabBar("ViewSwitch", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("HexView")) {
                    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                        ImGui::OpenPopup("context");
                    DrawContents(_rPalette.data(), _rPalette.size(), 0x0);
                    if (ContentsWidthChanged)
                    {
                        CalcSizes(s, _rPalette.size(), 0x0);
                        ImGui::SetWindowSize(ImVec2(s.WindowWidth, ImGui::GetWindowSize().y));
                    }

                    ImGui::EndTabItem();
                } // if

                if (ImGui::BeginTabItem("Color View")) {
                    int column_count = 2 ;

                    if (ImGui::BeginTable("Palette table", column_count, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV)) {
                        ImGui::TableNextColumn();

                        ImGui::PushID(0);
                        ImGui::BulletText("Background Palette", ImGui::GetContentRegionAvail().x);
                        ImGui::Image(_bgPaletteTex->GetTextureId(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * .5f)) ;
                        ImGui::PopID();

                        ImGui::TableNextColumn();
                        ImGui::PushID(1);
                        ImGui::BulletText("OBJ Palette", ImGui::GetContentRegionAvail().x);
                        ImGui::Image(_objPaletteTex->GetTextureId(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * .5f)) ;
                        ImGui::PopID();

                        ImGui::EndTable();
                    } // if

                    ImGui::EndTabItem();
                } // if

                ImGui::EndTabBar();
            } // if
        }

        ImGui::End();
    } // Draw()
} // gg_widget