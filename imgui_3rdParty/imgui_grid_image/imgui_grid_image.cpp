//
// Created by 0rzgg on 2022/7/4.
//

#include "imgui_grid_image.h"

#include <string>

#include <imgui.h>

namespace gg_widget {
    static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y) ;
    } // operator+

    static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) {
        return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y) ;
    } // operator-

    static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) {
        lhs.x += rhs.x ;
        lhs.y += rhs.y ;
        return lhs ;
    } // operator+=

    GridImage::GridImage(gg_gfx::tex_sp_t tex, ImVec2 gs, ImVec4 gc, float gt) :
        _tex(std::move(tex)),
        _image_size(_tex->Width(), _tex->Height()),
        _grid_size(gs),
        _grid_color(gc),
        _grid_thickness(gt)
    {

    }

    void GridImage::DrawContents(std::string name, ImVec2 size, ImGuiWindowFlags_ flags) {
        if (_grid_color.w > 0.0f)
            size += ImVec2(_grid_thickness * 2, _grid_thickness * 2) ;

        _scaleFactorX = size.x / _image_size.x ;
        _scaleFactorY = size.y / _image_size.y ;

        ImGui::BeginChild(name.c_str(), size, false, flags) ;
//        ImGui::Text("OnHovered: %d, callback: %d",
//            ImGui::IsItemHovered(),
//            onHovered_cb != nullptr
//        ) ;

        DrawGridImage() ;
        if (ImGui::IsWindowHovered() && onHovered_cb != nullptr) {
            onHovered_cb(this) ;
        } // if
        ImGui::EndChild() ;
    } // DrawContents()

    void GridImage::DrawGridImage() {
        struct ImRect {
            ImVec2 Min, Max ;
        };

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        _drawCursorPos = ImGui::GetCursorScreenPos() ;

        ImRect bb{.Min = _drawCursorPos, .Max = _drawCursorPos + ImGui::GetContentRegionAvail()};

        if (_grid_color.w > 0.0f)
        {
            drawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(_grid_color), 0.0f);

            drawList->AddImage(
                _tex->GetTextureId(),
                bb.Min + ImVec2(_grid_thickness, _grid_thickness),
                bb.Max - ImVec2(_grid_thickness, _grid_thickness),
                uv0, uv1, 0xffffffff
            );

            ImVec2 imgUpperLeft = bb.Min + ImVec2(_grid_thickness, _grid_thickness) ;
            ImVec2 imgBottomRight = bb.Max - ImVec2(_grid_thickness, _grid_thickness) ;

            // We were rounding all grid relative value to int to prevent sub pixel problem
            const int realGridWidth  = _grid_size.x * _scaleFactorX ;
            const int realGridHeight = _grid_size.y * _scaleFactorX ;

            const int gridColNum = ImGui::GetContentRegionAvail().x / realGridWidth ;
            const int gridRowNum = ImGui::GetContentRegionAvail().y / realGridHeight ;

            for (int drawHorizontal = 1 ; drawHorizontal < gridColNum ; ++drawHorizontal) {
                float gridPlotX = imgUpperLeft.x + drawHorizontal * realGridWidth ;
                drawList->AddLine(
                        ImVec2(gridPlotX, imgUpperLeft.y),
                        ImVec2(gridPlotX, imgBottomRight.y),
                        ImGui::GetColorU32(_grid_color)) ;
            } // for

            for (int drawVertical = 1 ; drawVertical < gridRowNum ; ++drawVertical) {
                float gridPlotY = imgUpperLeft.y + drawVertical * realGridHeight ;
                drawList->AddLine(
                        ImVec2(imgUpperLeft.x, gridPlotY),
                        ImVec2(imgBottomRight.x, gridPlotY),
                        ImGui::GetColorU32(_grid_color)) ;
            } // for
        } // if
        else
        {
            drawList->AddImage(_tex->GetTextureId(), bb.Min, bb.Max, uv0, uv1, ImGui::GetColorU32(_grid_color));
        } // else
    } // GridImage()
}