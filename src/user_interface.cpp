//
// Created by orzgg on 2019-07-29.
//

#include "user_interface.h"

#include <fstream>
#include <filesystem>

namespace gg_widget {
//    void* (*user_interface::fp_CreateTexture)(uint8_t *data, int w, int h, char fmt) = nullptr ;
//    void (*user_interface::fp_UpdateTexture)(int x, int y, int w, int h, void* texId, const uint8_t *data) = nullptr ;
//    void (*user_interface::fp_DeleteTexture)(void* texId) = nullptr ;

    user_interface::user_interface(unsigned int width, unsigned int height) :
            _W(width), _H(height)
    {
    }

    void user_interface::DrawWidgets() {
        for (auto wItr = widgetList.cbegin() ; wItr != widgetList.cend() ; ++wItr ) {
            ImGui::Begin(wItr->second->GetName().c_str(),
                        &(wItr->second->alive),
                        wItr->second->flags) ;

            wItr->second->Draw() ;
            ImGui::End() ;

            if (!wItr->second->StillAlive())
                widgetList.erase(wItr) ;
        } // for
    } // DrawWidgets()

    void user_interface::ReadFile(const std::string& path, gg_raw_data &buffer) {
        std::cout << "OPEN[" << path.c_str() << "]" << std::endl;

        namespace fs = std::filesystem;

        try
        {
            int binFileSize = fs::file_size(path);
            gg_raw_data rawData(binFileSize);

            std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);

            for (size_t t = 0; t < binFileSize; ++t)
            {
                rawData[t] = ifs.get();
            } // for

            buffer = std::move(rawData);
        }
        catch (fs::filesystem_error &e)
        {
            std::cerr << e.what() << std::endl;
        } // try-catch
    }
}
