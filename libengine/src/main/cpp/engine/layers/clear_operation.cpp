#include "engine/layers/clear_operation.hpp"

#include "engine/layers/clear_type.hpp"
#include "engine/layers/color.hpp"

namespace engine {
    namespace layers {
        clear_operation clear_operation::colorDepth(const color& color, float depth) noexcept {
            return {
                clear_type::COLOR | clear_type::DEPTH,
                color,
                0,
                depth};
        }

        clear_operation clear_operation::noop() noexcept {
            return {clear_type::NONE};
        }
    }
}