#include "graphics/blend_state_info.hpp"

namespace graphics {
    template<>
    blend_state_info defaults() {
        return {
            false,
            graphics::blend_equation::ADD, graphics::blend_equation::ADD,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            0xFFFF};
    }
}