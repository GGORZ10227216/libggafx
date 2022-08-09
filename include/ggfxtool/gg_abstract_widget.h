//
// Created by orzgg on 2022-01-06.
//

#include <string>
#include <imgui.h>

#ifndef LIBGAFX_GG_ABSTRACT_WIDGET_H
#define LIBGAFX_GG_ABSTRACT_WIDGET_H

namespace gg_widget {
    class user_interface;

    class gg_abstract_widget {
    public :
        friend class user_interface ;

        gg_abstract_widget(user_interface* parent, const char* wn, int w, int h, uint32_t serialNum) ;
        virtual ~gg_abstract_widget() = default ;

        virtual void Draw() = 0 ;
        void SetName(const char* newName) {name = newName;}
        std::string GetName() {return name;}

        bool StillAlive() {return alive;}

    protected:
        std::string name = "(empty)" ;
        ImGuiWindowFlags_ flags = ImGuiWindowFlags_None ;

        int width, height ;
        bool isVisible = false ;
        bool alive = false ;

        user_interface* const _parent ;
    };
}

#endif //LIBGAFX_GG_ABSTRACT_WIDGET_H
