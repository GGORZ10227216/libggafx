#include <iostream>

#include <videodump_viewer.h>
#include <gg_palette_viewer.h>

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 768

int main(int argv, char** args) {
    using PaletteViewer = gg_widget::gg_palette_viewer;
    gg_widget::gg_videodump_viewer<gg_widget::SDL_GL_interface> interface(SCREEN_WIDTH, SCREEN_HEIGHT) ;

    interface.StartUI();

    return 0;
} // main()
