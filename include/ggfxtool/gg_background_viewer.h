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

#ifndef LIBGAFX_GG_BACKGROUND_VIEWER_H
#define LIBGAFX_GG_BACKGROUND_VIEWER_H

namespace gg_widget
{
    enum BG_MODE {TEXT, TRANSFORM} ;

    class gg_background_viewer : public gg_abstract_widget, public MemoryEditor
    {
    public:
        gg_background_viewer() = delete;
        gg_background_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum);

        void Draw() override;

        gg_raw_data rVramRaw;

    private:
        int currentbgNumber = 0 ;

        const char* bgName[4] = {
            "#0",
            "#1",
            "#2",
            "#3"
        };

        void DrawBackground(int bgNum, BG_MODE bgMode) ;
        std::array<std::shared_ptr<gg_gfx::gg_texture>, 4> _bgTex;
        std::shared_ptr<gg_gfx::ppu> _pPpu;

        double fps = 0 ;

        double t = 0, fc = 0 ;
    };
} // gg_widget

#endif //LIBGAFX_GG_BACKGROUND_VIEWER_H
