#include "engine/layers/scene_layer_info.hpp"

namespace engine {
    namespace layers {
        scene_layer_info scene_layer_info::init(const basic_image_layer_info& info) noexcept {
            auto out = scene_layer_info();

            out.type = layer_type::BASIC_IMAGE_LAYER;
            out.info.basicImageLayer = info;

            return out;
        }
    }
}