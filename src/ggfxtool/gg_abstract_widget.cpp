//
// Created by orzgg on 2022-01-06.
//

#include <gg_abstract_widget.h>
#include <fmt/format.h>

namespace gg_widget {
    gg_abstract_widget::gg_abstract_widget(user_interface* parent, const char* wn, int w, int h, uint32_t serialNum):
        _parent(parent),
        name(fmt::format("{}{}", wn, serialNum)),
        width(w),
        height(h)
    {
        alive = true ;
    }
}
