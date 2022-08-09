//
// Created by orzgg on 2022-01-10.
//
#include <iostream>
#include <memory>
#include <functional>

#include <imgui.h>
#include <imgui_memory_editor.h>

#include <gg_type.h>
#include <gg_abstract_widget.h>
#include <gg_texture.h>

#ifndef LIBGAFX_GG_SPRITE_VIEWER_H
#define LIBGAFX_GG_SPRITE_VIEWER_H

namespace gg_gfx {
    class ppu ;
    class OAM ;
}

namespace gg_widget {

    class gg_sprite_viewer : public gg_abstract_widget {
    public :
        gg_sprite_viewer() = delete ;
        gg_sprite_viewer(user_interface* parent, std::shared_ptr<gg_gfx::ppu> pPpu, uint32_t serialNum) ;

        void Draw() override ;

    private:
        void RedrawFullSprite() ;

        enum { MAX_OAM_NUM = 128 };

        gg_gfx::OAM &_rOam ;
        gg_raw_data &_rVram ;

        int _selected = -1 ;

        std::shared_ptr<gg_gfx::gg_texture> _ggFullSprite;
        std::shared_ptr<gg_gfx::gg_texture> _ggSpriteTile;

        std::array<gg_gfx::tex_sp_t, MAX_OAM_NUM> _spriteTextures ;
        std::shared_ptr<gg_gfx::ppu> _pPpu ;
        user_interface *const _parent = nullptr ;

        int tx = 0, ty = 0 ;
    };
} // gg_widget

#endif //LIBGAFX_GG_SPRITE_VIEWER_H
