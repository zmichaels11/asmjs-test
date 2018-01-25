#pragma once

namespace engine {
    namespace layers {
        enum class scene_layer_hint : unsigned int {
            CLEAR = 0x1,
            SCISSOR = 0x2,
            VIEWPORT = 0x4
        };

        constexpr scene_layer_hint operator| (scene_layer_hint lhs, scene_layer_hint rhs) {
            return static_cast<scene_layer_hint> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
        }

        constexpr scene_layer_hint operator& (scene_layer_hint lhs, scene_layer_hint rhs) {
            return static_cast<scene_layer_hint> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
        }

        constexpr scene_layer_hint& operator|= (scene_layer_hint& lhs, scene_layer_hint rhs) {
            return lhs = lhs | rhs;
        }

        constexpr scene_layer_hint& operator&= (scene_layer_hint& lhs, scene_layer_hint rhs) {
            return lhs = lhs & rhs;
        }
    }
}