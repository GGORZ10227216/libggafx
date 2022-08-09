//
// Created by orzgg on 2022-01-13.
//

#include <cstdint>
#include <memory>
#include <glad/glad.h>

#include <gg_texture.h>

#ifndef LIBGAFX_GG_GL_TEXTURE_H
#define LIBGAFX_GG_GL_TEXTURE_H

namespace gg_gfx {
    struct gg_gl_texture : public gg_texture {
        gg_gl_texture() = delete ;
        gg_gl_texture(int w, int h, GLuint internalFormat, GLuint format, GLuint dataType) ;
        gg_gl_texture(int w, int h, GLuint internalFormat, GLuint format, GLuint dataType, const gg_raw_data& raw) ;

        virtual ~gg_gl_texture() ;

        void UpdateData(const gg_raw_data& buffer) override;
        void Update() override;

        GLuint _internalFormat, _format, _dataType ;
    };
}

#endif //LIBGAFX_GG_GL_TEXTURE_H
