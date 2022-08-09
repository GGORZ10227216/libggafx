//
// Created by orzgg on 2022-01-10.
//

#include <iostream>
#include <chrono>

#include <gg_background_viewer.h>
#include <imgui_grid_image.h>

#include <io_enum.h>
#include <bit_manipulate.h>

#include <fmt/printf.h>
#include <glad/glad.h>

namespace gg_widget
{
    gg_background_viewer::gg_background_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum) :
        gg_abstract_widget(parent, "Background", 240, 320, serialNum),
        _pPpu(pPpu),
        rVramRaw(pPpu->rVram)
    {
        for (int i = 0 ; i < 4 ; ++i) {
            _bgTex[ i ] = std::make_shared<gg_gfx::gg_texture>(
                _parent, 240, 160
            ); 
        } // for

        DrawBackground(currentbgNumber, BG_MODE::TEXT);
    }

    void gg_background_viewer::DrawBackground(int bgNum, BG_MODE bgMode) {
        using namespace gg_gfx;
        uint16_t bgcnt = BG_CNT::GetBGCNT(_pPpu->rIoReg.data(), bgNum) ;

        int  mapBlockNum = BG_CNT::MapBaseBlockNumber(bgcnt) ;
        int tileBlockNum = BG_CNT::TileBaseBlockNumber(bgcnt) ;

        const int tilePerRow = 32, tilePerCol = 32 ;
        if (bgMode == TEXT) {
            /*Text mode BG layer*/
//            uint16_t*  mapAddrBase = (uint16_t*)(_pPpu->rVram.data() +  0x800*mapBlockNum) ;
//
//            for (int currentRowIdx = 0 ; currentRowIdx < tilePerCol ; ++currentRowIdx) {
//                for (int tileRowNum = 0 ; tileRowNum < 8 ; ++tileRowNum) {
//                    for (int currentColIdx = 0 ; currentColIdx < tilePerRow ; ++currentColIdx) {
//                        uint16_t mapData = *(mapAddrBase + tilePerRow*currentRowIdx + currentColIdx);
//                        uint16_t tileNum = mapData & 0x1ff ;
//
//                        int paletteNum = gg_core::TestBit(bgcnt, 7) ? -1 : gg_core::BitFieldValue<12, 4>(mapData) ;
//
//                        uint8_t* tileAddrBase = _pPpu->GetBgTileDataPtr(gg_gfx::BG_TYPE::TEXT,tileBlockNum, bgNum, tileNum) ;
//
//                        _pPpu->DrawBGTileLine(
//                            tileRowNum,
//                            paletteNum,
//                            tileAddrBase,
//                            (uint32_t*)_bgTex[ bgNum ]->GetRawDataXY(currentColIdx*8, currentRowIdx*8 + tileRowNum)
//                        ) ;
//                    } // for
//                } // for
//            } // for
            for (int i = 0 ; i < 160 ; ++i)
                _pPpu->DrawTextBackgroundScanline(bgNum, i) ;
        } // if
        else {
            /*Rotate/Scale BG layer*/
            uint8_t*  mapAddrBase = _pPpu->rVram.data() +  0x800*mapBlockNum ;

            for (int currentRowIdx = 0 ; currentRowIdx < 64 ; ++currentRowIdx) {
                for (int tileRowNum = 0 ; tileRowNum < 8 ; ++tileRowNum) {
                    for (int currentColIdx = 0 ; currentColIdx < 64 ; ++currentColIdx) {
                        uint16_t mapData = *(mapAddrBase + 64*currentRowIdx + currentColIdx);
                        uint16_t tileNum = mapData & 0xff ;

                        int paletteNum = -1 ;

                        uint8_t* tileAddrBase = _pPpu->GetBgTileDataPtr(gg_gfx::BG_TYPE::AFFINE, tileBlockNum, bgNum, tileNum) ;

                        _pPpu->DrawBGTileLine(
                                tileRowNum,
                                paletteNum,
                                tileAddrBase,
                                (uint32_t*)_bgTex[ bgNum ]->GetRawDataXY(currentColIdx*8, currentRowIdx*8 + tileRowNum)
                        ) ;
                    } // for
                } // for
            } // for
        } // else 

        _bgTex[ bgNum ]->Update() ;
    } // gg_background_viewer::DrawBackground()

    void gg_background_viewer::Draw()
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
                    GridImage gridImage(_bgTex[ currentbgNumber ], ImVec2(8,8), ImVec4(0.5, 0.5, 0.5, 1), 1.0f);

                    gridImage.onHovered_cb = [](const GridImage* self) {
                        ImGuiIO& io = ImGui::GetIO();
                        ImGui::BeginTooltip();

                        const ImVec2& upper_left = self->GetUpperLeftCord() ;
                        const ImVec2 scaledGridSize = self->GetScaledGridSize() ;
                        const float relativeCursorX = io.MousePos.x - upper_left.x ;
                        const float relativeCursorY = io.MousePos.y - upper_left.y ;
                        ImGui::Text("Cursor: (%f, %f), grid: (%d, %d)",
                                    relativeCursorX,
                                    relativeCursorY,
                                    static_cast<int>(relativeCursorX / scaledGridSize.x),
                                    static_cast<int>(relativeCursorY / scaledGridSize.y)

                        ) ;
//                        float region_sz = 32.0f;
//                        float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
//                        float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
//                        float zoom = 4.0f;
//                        if (region_x < 0.0f) { region_x = 0.0f; }
//                        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
//                        if (region_y < 0.0f) { region_y = 0.0f; }
//                        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
//                        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
//                        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
//                        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
//                        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
//                        ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                        ImGui::EndTooltip();
                    };

                    gridImage.DrawContents("TileTexture", ImVec2(480, 320), ImGuiWindowFlags_NoScrollbar);
//                    ImGui::BeginChild("TileTexture", ImVec2(482, 322), false, ImGuiWindowFlags_NoScrollbar);
//
//                    float viewableAreaWidth = ImGui::GetContentRegionAvail().x - 2 ;
//                    float viewableAreaHeight = ImGui::GetContentRegionAvail().y - 2 ;
//
//                    ImGui::GridImage(
//                            _bgTex[ currentbgNumber ]->GetTextureId(),
//                            ImVec2(viewableAreaWidth,viewableAreaHeight),
//                            ImVec2(viewableAreaWidth/30, viewableAreaHeight/20),
//                            ImVec2(0,0),
//                            ImVec2(1,1),
//                            ImVec4(1,1,1,1),
//                            ImVec4(0.5,0.5,0.5,1)
//                    );
//                    ImGui::EndChild();

//                    ImGuiTableFlags gridFlags = ImGuiTableFlags_BordersInner |
//                                                ImGuiTableFlags_BordersOuter ;

//                    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0.5));
//                    ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ImVec4(1.0, 0, 0, 1)) ;
//                    if (ImGui::BeginTable("Grid", 30, gridFlags)) {
//                        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
//                        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
//
//                        for (int i = 0 ; i < 20 ; ++i) {
//                            ImGui::TableNextRow();
//                            for (int j = 0 ; j < 30 ; ++j) {
//                                float texCrodX = j * 8, texCrodY = i * 8 ;
//                                ImVec2 uv0 = ImVec2(static_cast<float>(texCrodX)/240, static_cast<float>(texCrodY)/160) ;
//                                ImVec2 uv1 = ImVec2(static_cast<float>(texCrodX + 8)/240, static_cast<float>(texCrodY + 8)/160) ;
//
//                                ImGui::TableSetColumnIndex(j);
//                                ImGui::Image(_bgTex[currentbgNumber]->GetTextureId(),
//                                             ImVec2(ImGui::GetContentRegionAvail().x, 16), uv0, uv1);
//                            } // for
//                        } // for
//
//
//                        ImGui::EndTable() ;
//                    } // if
//                    ImGui::PopStyleVar() ;
//                    ImGui::PopStyleColor() ;

                    if (ImGui::BeginCombo("##Palette select", bgName[currentbgNumber], 0))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(bgName); n++)
                        {
                            const bool is_selected = (currentbgNumber == n);
                            if (ImGui::Selectable(bgName[n], is_selected))
                            {
                                currentbgNumber = n;
                                DrawBackground(currentbgNumber, BG_MODE::TEXT);
                            } // if

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }

                        ImGui::EndCombo();
                    } // if
                    ImGui::EndTabItem();
                } // if

                if (ImGui::BeginTabItem("Screen View"))
                {
//                    ImGui::BeginChild("Screen", ImVec2(480, 320), false, ImGuiWindowFlags_NoScrollbar);
//                    auto start = std::chrono::steady_clock::now() ;
//                    CALLGRIND_START_INSTRUMENTATION;
//                    CALLGRIND_TOGGLE_COLLECT;

                    for (int i = 0 ; i < 160 ; ++i) {
                        _pPpu->DrawScreenLine(i) ;
                    } // for
//                    CALLGRIND_TOGGLE_COLLECT;
//                    CALLGRIND_STOP_INSTRUMENTATION;

//                    exit(0);

                    for (int y = 0 ; y < 160 ; ++y) {
                        uint32_t* rowStart = reinterpret_cast<uint32_t*>(_bgTex[0]->GetRawDataXY(0, y)) ;
                        for (int x = 0 ; x < 240 ; ++x) {
//                            *rowStart = _pPpu->PixelColorCorrectionFunc(_pPpu->frameBuffer[240*y + x]) ;
                            uint16_t& input = _pPpu->frameBuffer[240*y + x] ;
                            uint8_t r, g, b, a;

                            r = (input & 0x1f) << 3 ;
                            g = (input & 0x3e0) >> 2 ;
                            b = (input & 0x7c00) >> 7 ;
                            a = 0xff ;

                            *rowStart = r | (g << 8) | (b << 16) | (a << 24) ;
                            ++rowStart ;
                        } // for
                    } // for

                    _pPpu->fcnt += 1 ;
                    _pPpu->rLast =  std::chrono::steady_clock::now() ;
                    std::chrono::duration<double> diff = _pPpu->rLast - _pPpu->rStart ;

                    if (diff.count() > 1.0) {
                        fps = _pPpu->fcnt / diff.count();
                        _pPpu->rStart = _pPpu->rLast ;
                        _pPpu->fcnt = 0 ;
                    } // if

                    ImGui::Text("fps: %lf", fps);

                    _bgTex[0]->Update() ;

                    ImGui::Image(_bgTex[0]->GetTextureId(), ImVec2(480, 320));
//                    ImGui::EndChild();
                    ImGui::EndTabItem();
                } // if
            }     // if

            ImGui::EndTabBar();
        }

        ImGui::End();
    } // Draw()
} // gg_widget