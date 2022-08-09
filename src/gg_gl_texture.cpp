//
// Created by orzgg on 2022-01-13.
//

//
// Created by orzgg on 2022-01-06.
//

#include <iostream>
//#include <gg_gl_texture.h>

namespace gg_gfx {
    // TODO: refactor the repeat part of code
//
//    gg_gl_texture::gg_gl_texture(int w, int h, GLuint internalFormat, GLuint format, GLuint dataType,
//                                 const gg_raw_data &raw) :
//            gg_texture(w, h, raw),
//            _internalFormat(internalFormat),
//            _format(format),
//            _dataType(dataType) {
//
//        if (!rawData.empty()) {
//            if (format == GL_RGBA)
//                pixelSize = 4 ;
//            else
//                pixelSize = 3 ;
//
//            glGenTextures(1, &texId);
//            glBindTexture(GL_TEXTURE_2D, texId);
//
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//            glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, width, height, 0, _format, _dataType, rawData.data());
//        } // if
//        else {
//            std::cerr << "Could not create opengl texture" << std::endl;
//        } // else
//    } // gg_texture(int w, int h)
//
//    gg_gl_texture::gg_gl_texture(int w, int h, GLuint internalFormat, GLuint format, GLuint dataType) :
//            gg_texture(w, h),
//            _internalFormat(internalFormat),
//            _format(format),
//            _dataType(dataType) {
//
//        if (!rawData.empty()) {
//            if (format == GL_RGBA)
//                pixelSize = 4 ;
//            else
//                pixelSize = 3 ;
//
//            glGenTextures(1, &texId);
//            glBindTexture(GL_TEXTURE_2D, texId);
//
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//            glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, width, height, 0, _format, _dataType, rawData.data());
//        } // if
//        else {
//            std::cerr << "Could not create opengl texture" << std::endl;
//        } // else
//    } // gg_texture(int w, int h)
//

//    void gg_gl_texture::Update() {
//        glBindTexture(GL_TEXTURE_2D, texId);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, _format, _dataType, rawData.data());
//    } // Update()
//
//    gg_gl_texture::~gg_gl_texture() {
//        glDeleteTextures(1, &texId);
//    } // ~gg_gl_texture()
}