//
// Created by orzgg on 2022-01-06.
//

#include <cstdint>
#include <memory>

#include <gg_type.h>

#ifndef LIBGAFX_GG_TEXTURE_H
#define LIBGAFX_GG_TEXTURE_H

namespace gg_widget {
    class user_interface ;
} // gg_widget

namespace gg_gfx {
    using Framework = gg_widget::user_interface ;

    struct gg_texture {
        gg_texture() = delete ;

        gg_texture(Framework* framework, int w, int h, gg_raw_data&& raw);
        gg_texture(Framework* framework, int w, int h);

        virtual ~gg_texture();

        void* GetTextureId() { return (void*)texId ; } // GetTextureId()

        int Width() { return width ;}
        int Height() { return height ;}

        // These two update data function need to be empty if this is a software texture
        void Update() ;

        auto GetRawData() { return rawData.data() ; }
        uint8_t* GetRawDataXY(int x, int y) ;

        size_t GetRawSize() { return rawData.size() ; }

        void* texId ;
        gg_raw_data rawData ;

        int width, height, pixelSize = 4 ;
    private:
        Framework* const _framework = nullptr ;
    };

    using tex_sp_t = std::shared_ptr<gg_gfx::gg_texture> ;
    using tex_up_t = std::unique_ptr<gg_gfx::gg_texture> ;
}

#endif //LIBGAFX_GG_TEXTURE_H
