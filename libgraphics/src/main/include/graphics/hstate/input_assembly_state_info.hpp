#pragma once

#include "graphics/hstate/state.hpp"

namespace graphics {    
    struct input_assembly_state_info {
        bool primitiveRestartEnabled;
    
        static input_assembly_state_info defaults() noexcept;                    
    };    

    template<>
    void apply(const input_assembly_state_info& info) noexcept;
}