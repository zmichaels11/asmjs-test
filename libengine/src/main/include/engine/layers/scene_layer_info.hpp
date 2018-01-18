#pragma once

#include "engine/layers/basic_image_layer.hpp"
#include "engine/layers/basic_sprite_layer.hpp"
#include "engine/layers/bounds.hpp"
#include "engine/layers/clear_operation.hpp"
#include "engine/layers/layer_type.hpp"
#include "engine/layers/text_layer_info.hpp"
#include "engine/layers/tile_layer_info.hpp"

namespace engine {
    namespace layers {
        struct scene_layer_info {
            layer_type type;
            clear_operation clear;
            engine::layers::bounds<int, int, int, int> scissor;
            engine::layers::bounds<int, int, unsigned int, unsigned int> viewport;
            
            union {
                basic_image_layer_info basicImageLayer;
                basic_sprite_layer_info basicSpriteLayer;                
                text_layer_info textLayer;
                tile_layer_info tileLayer;            
            } info;
        };
    }
}