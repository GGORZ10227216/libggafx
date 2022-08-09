//
// Created by 0rzgg on 2022/7/4.
//

#include <imgui.h>

#include <functional>
#include <memory>

#include <gg_texture.h>

#ifndef LIBGAFX_IMGUI_GRID_IMAGE_H
#define LIBGAFX_IMGUI_GRID_IMAGE_H

namespace gg_widget {
    struct GridImage {
        GridImage(gg_gfx::tex_sp_t tex, ImVec2 gs, ImVec4 gc, float gt) ;
        void DrawContents(std::string name, ImVec2 size, ImGuiWindowFlags_ flags) ;

        [[nodiscard]] ImVec2 GetScaledGridSize() const {
            return ImVec2(_grid_size.x * _scaleFactorX, _grid_size.y * _scaleFactorY);
        } // GetScaledGridSize()

        const ImVec2& GetUpperLeftCord() const { return _drawCursorPos; }

        gg_gfx::tex_sp_t _tex ;
        ImVec2 _image_size, _grid_size ;
        ImVec4 _grid_color = ImVec4(0.8, 0.8, 0.8, 1) ;
        float _grid_thickness = 1.0f ;

        std::function<void(const GridImage*)> onHovered_cb ;

    private:
        void DrawGridImage() ;
        ImVec2 _drawCursorPos ;

        float _scaleFactorX = 1.0f;
        float _scaleFactorY = 1.0f;

        ImVec2 uv0 = ImVec2(0.0f, 0.0f);
        ImVec2 uv1 = ImVec2(1.0f, 1.0f);
    };
}

#endif //LIBGAFX_IMGUI_GRID_IMAGE_H
