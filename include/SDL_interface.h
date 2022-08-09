//
// Created by orzgg on 2019-07-28.
//

#ifndef GGBOY_MK2_SDL_INTERFACE_H
#define GGBOY_MK2_SDL_INTERFACE_H

#include <iostream>
#include <map>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <SDL.h>
#include <glad/glad.h>

#include <user_interface.h>

namespace gg_widget {
    class SDL_GL_interface : public gg_widget::user_interface {
    public:
        SDL_GL_interface() = delete ;
        virtual ~SDL_GL_interface() ;
        SDL_GL_interface(int width, int height) ;

        void StartUI() override ;

        int InitSDL() ;
        int InitOGL() ;
        int InitIMGUI() ;

        static void* CreateTexture(uint8_t *data, int w, int h, char fmt);
        static void UpdateTexture(int x, int y, int w, int h, void* texId, const uint8_t *data);
        static void DeleteTexture(void* texId) ;

        const char* glsl_version = "#version 150";
        SDL_WindowFlags window_flags ;
        SDL_Window* window;
        SDL_GLContext gl_context ;
    };
} // gg_widget

#endif //GGBOY_MK2_SDL_INTERFACE_H
