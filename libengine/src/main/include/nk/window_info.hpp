#pragma once

#include "nk/rect.hpp"
#include "nk/window_flag.hpp"

namespace nk {
    struct window_info {
        const char * title;
        const char * id;
        rect bounds;        
        window_flag flags;                
    };
}