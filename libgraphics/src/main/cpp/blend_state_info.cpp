#include "graphics/hstate/blend_state_info.hpp"

namespace graphics {    
    blend_state_info blend_state_info::defaults() noexcept {
        return {
            false,
            graphics::blend_equation::ADD, graphics::blend_equation::ADD,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            {true, true, true, true}};
    }

    blend_state_info blend_state_info::premultipliedAlpha() noexcept {
        return {
            true, 
            graphics::blend_equation::ADD, graphics::blend_equation::ADD,
            graphics::blend_function::ONE, graphics::blend_function::ONE_MINUS_SRC_ALPHA,
            graphics::blend_function::ONE, graphics::blend_function::ONE_MINUS_SRC_ALPHA,
            {true, true, true, true}};
    }
}