//
// Created by orzgg on 2022-01-10.
//

#include <memory>
#include <functional>

#include <imgui.h>
#include <imgui_memory_editor.h>

#include <gg_type.h>
#include <ppu.h>
#include <gg_abstract_widget.h>
//#include <gg_gl_texture.h>
#include <gg_texture.h>

#ifndef LIBGAFX_GG_PALETTE_VIEWER_H
#define LIBGAFX_GG_PALETTE_VIEWER_H

namespace gg_widget {
    class gg_palette_viewer : public gg_abstract_widget, public MemoryEditor {
    public :
        gg_palette_viewer() = delete ;
        gg_palette_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum) ;

        void Draw() override ;

    private:
        gg_raw_data& _rPalette ;

        std::unique_ptr<gg_gfx::gg_texture> _bgPaletteTex;
        std::unique_ptr<gg_gfx::gg_texture> _objPaletteTex;
    };
} // gg_widget

#endif //LIBGAFX_GG_PALETTE_VIEWER_H
