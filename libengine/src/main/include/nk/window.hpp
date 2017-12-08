#pragma once 

#include "nk/window_info.hpp"

namespace nk {    
    class window {
        bool _success;

    public:
        window(const window_info& info);

        ~window();

        explicit operator bool() const;
    };
}
