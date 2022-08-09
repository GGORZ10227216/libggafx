//
// Created by orzgg on 2019-07-29.
//

#ifndef GGBOY_MK2_USER_INTERFACE_H
#define GGBOY_MK2_USER_INTERFACE_H

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include <cstdint>

#include <gg_type.h>
#include <gg_abstract_widget.h>

namespace gg_widget {
    class user_interface {
    public :
        user_interface() = delete ;
        user_interface(unsigned int width, unsigned int height) ;
        virtual void StartUI() {}
        virtual ~user_interface() = default ;

        uint32_t GetWidgetSerialNumber() {return widgetSerialNum;}

        void AddWidget(std::unique_ptr<gg_abstract_widget> wPtr) {
            if (wPtr != nullptr) {
                widgetList[ wPtr->GetName() ] = std::move(wPtr) ;
                ++widgetSerialNum ;
            } // if
        }  // AddWidget()

        void RemoveWidget(const std::string& key) {
            if (widgetList.contains(key))
                widgetList.erase(key) ;
            else
                std::cerr << "Attempt to remove a widget which is not existed." << std::endl ;
        } // if

        void ReadFile(const std::string& path, gg_raw_data &buffer) ;

        // todo: pure software texture create&delete logic
        void* (*fp_CreateTexture)(uint8_t *data, int w, int h, char fmt) = nullptr ;
        void (*fp_UpdateTexture)(int x, int y, int w, int h, void* texId, const uint8_t *data) = nullptr ;
        void (*fp_DeleteTexture)(void* texId) = nullptr ;

    protected:
        std::map<std::string, std::unique_ptr<gg_widget::gg_abstract_widget>> widgetList ;

        // virtual bool Init() = 0 ;
        ImGuiIO* ioptr = nullptr ;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        unsigned int _W, _H ;

        uint32_t widgetSerialNum = 0 ;

        virtual void DrawWindow() = 0;
        virtual void DrawWidgets() ;
    };
}

#endif //GGBOY_MK2_USER_INTERFACE_H
