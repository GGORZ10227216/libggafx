//
// Created by orzgg on 2019-07-28.
//

#include "SDL_interface.h"
#include <gg_texture.h>
#include <ImFileDialog.h>

namespace gg_widget {
    SDL_GL_interface::SDL_GL_interface(int width, int height) :
            user_interface(width, height)
    {
        if ( InitSDL() | InitOGL() ) {
            std::cerr << "SDL_GL_interface initialize failed." << std::endl ;
            exit( -1 ) ;
        } // if

        InitIMGUI() ;

        fp_CreateTexture = &SDL_GL_interface::CreateTexture ;
        fp_DeleteTexture = &SDL_GL_interface::DeleteTexture ;
        fp_UpdateTexture = &SDL_GL_interface::UpdateTexture ;

        ifd::FileDialog::Instance().CreateTexture = fp_CreateTexture;
        ifd::FileDialog::Instance().DeleteTexture = fp_DeleteTexture;
    }

    int SDL_GL_interface::InitSDL() {
        if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) ) {
            std::cerr << "Error occurred when initializing SDL: " << SDL_GetError() << std::endl ;
            return -1 ;
        } // if

        return 0 ;
    } // SDL_GL_interface::InitSDL()

    int SDL_GL_interface::InitOGL() {
        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        window = SDL_CreateWindow("GGBoy! mkII", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _W, _H, window_flags);
        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(0); // Enable vsync

        gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress) ;
        return 0 ;
    } // SDL_GL_interface::InitOGL()

    int SDL_GL_interface::InitIMGUI() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ioptr = &(ImGui::GetIO()); (void)*ioptr;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version);
        return 0 ;
    } // SDL_GL_interface::InitIMGUI()

    void SDL_GL_interface::StartUI() {
        // Main loop
        bool done = false;

        while (!done) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                const uint8_t* keyboardStatus = SDL_GetKeyboardState( NULL ) ;
                if (event.type == SDL_QUIT)
                    done = true;
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                    done = true;
            } // while

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            DrawWindow() ;
            DrawWidgets() ;

            // Rendering
            ImGui::Render();
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }
    }

    void *SDL_GL_interface::CreateTexture(uint8_t *data, int w, int h, char fmt) {
        GLuint tex;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return (void *)tex;
    } // CreateTexture()

    void SDL_GL_interface::UpdateTexture(int x, int y, int w, int h, void* texId, const uint8_t *data) {
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint64>(texId));
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } // UpdateTexture()

    void SDL_GL_interface::DeleteTexture(void *tex) {
        GLuint texID = (uint64_t)tex;
        glDeleteTextures(1, &texID);
    } // DeleteTexture()

    SDL_GL_interface::~SDL_GL_interface() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    } // ~SDL_GL_interface()
} // gg_widget