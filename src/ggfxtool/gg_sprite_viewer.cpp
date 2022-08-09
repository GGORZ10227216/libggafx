//
// Created by 0rzgg on 2022/6/29.
//

#include <gg_sprite_viewer.h>

#include <fmt/format.h>
#include <ppu.h>
#include <oam.h>
#include "imgui_grid_image.h"

namespace gg_widget {
    gg_sprite_viewer::gg_sprite_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum) :
        gg_abstract_widget(parent, "Sprites", 240, 320, serialNum),
        _pPpu(pPpu),
        _parent(parent),
        _rOam(pPpu->rOam),
        _rVram(pPpu->rVram)
    {
//        _ggFullSprite = std::make_shared<gg_gfx::gg_texture>(parent, 64, 64) ;

        RedrawFullSprite() ;

        for (auto& tex : _spriteTextures) {
            tex = std::make_shared<gg_gfx::gg_texture>(parent, 64, 64) ;
        } // for

        for (int i = 0 ; i < 128 ; ++i) {
            gg_gfx::ObjAttr attr = _rOam.GetAttr(i); // copy it because we are debugging
            int spriteHeight = attr.objRect().h ;

            for (int y = 0 ; y < spriteHeight ; ++y) {
                pPpu->DrawSpriteLine(
                    _spriteTextures[ i ]->GetRawDataXY(0, y),
                    y,
                    &attr
                ) ;
            } // for

            _spriteTextures[ i ]->Update() ;
        } // for

//        _ggFullSprite->Update() ;
//
//        for (int i = 0 ; i < 8 ; ++i) {
//            for (int j = 0 ; j < 8 ; ++j) {
//                uint32_t *pixelAddr = (uint32_t*)_ggSpriteTile->GetRawDataXY(i, j);
//                *pixelAddr = 0xff0000ff ;
//            } // for
//        } // for
//
//        _ggSpriteTile->Update() ;
    }

    void gg_sprite_viewer::Draw() {
        if (ImGui::Begin(name.c_str(), &alive, ImGuiWindowFlags_NoScrollbar)) {
            if (ImGui::BeginChild("OAM List", ImVec2(150, 0), true)) {
                for (int i = 0 ; i < MAX_OAM_NUM ; ++i) {
                    std::string label = fmt::format("OAM #{}", i) ;
                    if (ImGui::Selectable(label.c_str(), i == _selected)) {
                        _selected = i ;
                        const gg_gfx::ObjAttr& selectedObj = _pPpu->rOam.GetAttr(_selected) ;
                        gg_gfx::ObjRect objRect = selectedObj.objRect() ;

                        int spriteHeight = selectedObj.objRect().h ;
                        int charaName = selectedObj.characterName() ;
                        tx = (charaName % 0x20)*8 ;
                        ty = (charaName / 0x20)*8 ;
                    } // if

                    if (ImGui::IsItemHovered()) {
                        const gg_gfx::ObjAttr& attr = _rOam.GetAttr( i );
                        ImGui::BeginTooltip();
                        ImGui::Text("Sprite #%d", i) ;
                        ImGui::Image(
                            _spriteTextures[ i ]->GetTextureId(),
                            ImVec2(64, 64)
                        ) ;

                        ImGui::SameLine() ;
                        ImGui::Text("Character name: 0x%x", attr.characterName());

                        ImGui::Separator();

                        ImGui::Text("Address: 0x%x", 0x700'0000 + 0x8*i) ;
                        ImGui::Text("Size: (%d, %d)", attr.objRect().w, attr.objRect().h) ;
                        ImGui::Text("Color format: %s",
                            attr.pixelSize() == gg_gfx::NIBBLE ? "4bpp" : "8bpp"
                        ) ;
                        ImGui::Text("H flip: %s",
                                    attr.H_Flip() ? "Yes" : "No"
                        ) ;
                        ImGui::Text("V flip: %s",
                                    attr.V_Flip() ? "Yes" : "No"
                        ) ;
                        ImGui::EndTooltip() ;
                    } // if
                } // for
            } // if

            ImGui::EndChild() ;
            ImGui::SameLine() ;

            if (ImGui::BeginChild("OAM Detail", ImGui::GetContentRegionAvail())) {
                if (ImGui::BeginTabBar("ViewSwitch", ImGuiTabBarFlags_None)) {
                    if (ImGui::BeginTabItem("Full tile view")) {
                        GridImage gridImage(_ggSpriteTile, ImVec2(8,8), ImVec4(0.5, 0.5, 0.5, 1), 1.0f);

                        gridImage.onHovered_cb = [&](const GridImage* self) {
                            ImGuiIO& io = ImGui::GetIO();
                            ImGui::BeginTooltip();

                            const ImVec2& upper_left = self->GetUpperLeftCord() ;
                            const ImVec2 scaledGridSize = self->GetScaledGridSize() ;
                            const float relativeCursorX = io.MousePos.x - upper_left.x ;
                            const float relativeCursorY = io.MousePos.y - upper_left.y ;
                            ImGui::Text("Cursor: (%f, %f), grid: (%d, %d), t:(%d,%d)",
                                        relativeCursorX,
                                        relativeCursorY,
                                        static_cast<int>(relativeCursorX / scaledGridSize.x),
                                        static_cast<int>(relativeCursorY / scaledGridSize.y),
                                        tx,
                                        ty
                            ) ;

                            ImGui::EndTooltip();
                        };

                        gridImage.DrawContents("TileTexture", ImVec2(256, 256), ImGuiWindowFlags_NoScrollbar);

                        ImGui::EndTabItem();
                    } // if

                    if (ImGui::BeginTabItem("Sprite View")) {
//                        if (ImGui::BeginTable("Attribute Layout", 3)) {
//                            ImGui::TableNextColumn();
//                            ImGui::PushID(0) ;
//                            ImGui::BulletText("Attribute #0") ;
//                            ImGui::PopID() ;
//
//                            ImGui::TableNextColumn();
//                            ImGui::PushID(1) ;
//                            ImGui::BulletText("Attribute #1") ;
//                            ImGui::PopID() ;
//
//                            ImGui::TableNextColumn();
//                            ImGui::PushID(2) ;
//                            ImGui::BulletText("Attribute #2") ;
//                            ImGui::PopID() ;
//
//                            ImGui::EndTable() ;
//                        } // if

                        ImGui::EndTabItem();
                    } // if

                    ImGui::EndTabBar();
                } // if

                ImGui::EndChild();
            } // if

            ImGui::End();
        } // if
    } // Draw()

    void gg_sprite_viewer::RedrawFullSprite() {
        if (_ggSpriteTile == nullptr)
            _ggSpriteTile = std::make_shared<gg_gfx::gg_texture>(_parent, 256, 256) ;

        for (int i = 0 ; i < 0x400 ; ++i) {
            for (int tileX = 0 ; tileX < 8 ; ++tileX) {
                for (int tileY = 0 ; tileY < 8 ; ++tileY) {
                    uint16_t pixelColor = _pPpu->GetTilePixelColor(gg_gfx::NIBBLE, i, tileX, tileY, 3, true);
                    uint32_t* plot = (uint32_t*)_ggSpriteTile->GetRawDataXY((i % 0x20)*8 + tileX, (i / 0x20)*8 + tileY) ;
                    *plot = gg_utility::RGB555_TO_RGB8888(pixelColor);
                } // for
            } // for
        } // for

        _ggSpriteTile->Update() ;
    } // gg_sprite_viewer::RedrawFullSprite()
} // gg_widget