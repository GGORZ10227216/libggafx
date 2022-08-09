//
// Created by orzgg on 2022-01-11.
//

#include <functional>

#include <SDL_interface.h>
#include <gg_palette_viewer.h>
#include <gg_tile_viewer.h>
#include <gg_background_viewer.h>
#include <gg_sprite_viewer.h>
#include <ppu.h>

#ifndef LIBGAFX_VIDEODUMP_VIEWER_H
#define LIBGAFX_VIDEODUMP_VIEWER_H

namespace gg_widget {
    template <typename T>
    class gg_videodump_viewer : public T {
    public :
        gg_videodump_viewer() = delete ;
        virtual ~gg_videodump_viewer() = default;

        gg_videodump_viewer(int width, int height) :
                T(width, height)
        {
            std::pair<const char *, gg_raw_data &> binPaths[4] = {
                    {"io.bin", ioReg},
                    {"palette.bin", palette},
                    {"vram.bin", vram},
                    {"oam.bin", oam}
            };

            for (auto p : binPaths) {
                this->ReadFile(p.first, p.second) ;
            } // for
        } // videodump_viewer()

    protected :
        virtual void DrawWindow() {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Tools"))
                {
                    if (ImGui::BeginMenu("Video..."))
                    {
                        if (ImGui::MenuItem("Palette viewer"))
                        {
                            this->AddWidget(
                                std::make_unique<gg_widget::gg_palette_viewer>(
                                    this,
                                    pPpu,
                                    this->GetWidgetSerialNumber()
                                )
                            );
                        } // if
                    else if (ImGui::MenuItem("Tile viewer"))
                    {
                        this->AddWidget(std::make_unique<gg_widget::gg_tile_viewer>(this, pPpu, this->GetWidgetSerialNumber()));
                    } // else if
                    else if (ImGui::MenuItem("BG viewer"))
                    {
                        this->AddWidget(std::make_unique<gg_widget::gg_background_viewer>(this, pPpu, this->GetWidgetSerialNumber()));
                    } // else if
                    else if (ImGui::MenuItem("Sprites")) {
                        this->AddWidget(std::make_unique<gg_widget::gg_sprite_viewer>(
                            this, pPpu, this->GetWidgetSerialNumber()
                        )) ;
                    } // else if

                        ImGui::EndMenu();
                    } // if

                    ImGui::EndMenu();
                } // if

                ImGui::EndMainMenuBar();
            } // if

            if (pPpu == nullptr)
            {
                pPpu = std::make_shared<gg_gfx::ppu>(ioReg, palette, vram, oam);
            } // if
        }     // DrawContents()

    private :
        bool isFileDialogOpened = false ;

        std::shared_ptr<gg_gfx::ppu> pPpu ;
        gg_raw_data ioReg ;
        gg_raw_data palette ;
        gg_raw_data vram ;
        gg_raw_data oam ;
    };
}

#endif //LIBGAFX_VIDEODUMP_VIEWER_H
