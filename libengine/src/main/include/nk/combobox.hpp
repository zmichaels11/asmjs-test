#pragma once

#include "nk/combobox_info.hpp"

namespace nk {
    class combobox_color {
        bool _selected;
        
    public:
        combobox_color(const combobox_color_info& info);

        ~combobox_color();

        explicit operator bool() const;
    };
}