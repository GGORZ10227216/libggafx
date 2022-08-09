//
// Created by orzgg on 2022-01-16.
//

#include <cstdint>
#include <glad/glad.h>

#include <iostream>

#ifndef LIBGAFX_GG_GFX_UTILITY_H
#define LIBGAFX_GG_GFX_UTILITY_H

namespace gg_utility {
    static uint32_t RGB555_TO_RGB8888(uint16_t input) {
        uint8_t r, g, b, a;

        r = (input & 0x1f) << 3 ;
        g = (input & 0x3e0) >> 2 ;
        b = (input & 0x7c00) >> 7 ;
        a = 0xff ;

        return r | (g << 8) | (b << 16) | (a << 24) ;
    } // RGB555_TO_RGB8888()
} // gg_utility

#endif //LIBGAFX_GG_GFX_UTILITY_H
