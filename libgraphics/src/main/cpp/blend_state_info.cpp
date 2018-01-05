#include "graphics/blend_state_info.hpp"

namespace graphics {
    template<>
    blend_state_info defaults() noexcept {
        return {
            false,
            graphics::blend_equation::ADD, graphics::blend_equation::ADD,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            {true, true, true, true}};
    }
}