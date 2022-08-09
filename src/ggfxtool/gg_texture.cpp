//
// Created by orzgg on 2022-01-06.
//

#include <iostream>

#include <gg_texture.h>
#include <glad/glad.h>

#include <user_interface.h>

namespace gg_gfx {
    uint8_t* gg_texture::GetRawDataXY(int x, int y) {
        return (rawData.data() + pixelSize*width*y + pixelSize*x) ;
    }

    gg_texture::gg_texture(Framework* framework, int w, int h, gg_raw_data&& raw) :
        width(w), height(h), _framework(framework)
    {
        rawData = std::move(raw) ;
        texId = _framework->fp_CreateTexture(rawData.data(), width, height, 0) ;
    } // gg_texture(int w, int h)

    gg_texture::gg_texture(Framework* framework, int w, int h) :
        width(w), height(h), _framework(framework)
    {
        // fixme: just using RGBA pixel size
        /* Create an empty texture */
        rawData.resize(width*height*4) ;
        texId = _framework->fp_CreateTexture(rawData.data(), width, height, 0) ;
    } // gg_texture(int w, int h)

    void gg_texture::Update() {
        _framework->fp_UpdateTexture(0, 0, width, height, texId, rawData.data()) ;
    } // Update()

    gg_texture::~gg_texture() {
        _framework->fp_DeleteTexture(texId) ;
    } // ~gg_texture ;
}