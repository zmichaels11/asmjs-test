#pragma once

#include "engine/layers/background_layer.hpp"
#include "engine/layers/basic_sprite_layer.hpp"
#include "engine/layers/bounds.hpp"
#include "engine/layers/clear_operation.hpp"
#include "engine/layers/layer_type.hpp"
#include "engine/layers/scene_layer_hint.hpp"
#include "engine/layers/text_layer_info.hpp"
#include "engine/layers/tile_layer_info.hpp"

namespace engine {
    namespace layers {
        struct scene_layer_info {
            layer_type type;

            struct extension_operations_t {
                scene_layer_hint hints;
                clear_operation clear;
                engine::layers::bounds<int, int> scissor;
                engine::layers::bounds<int, int> viewport;
            } ext;
            
            union {
                background_layer_info backgroundLayer;
                basic_sprite_layer_info basicSpriteLayer;                
                text_layer_info textLayer;
                tile_layer_info tileLayer;            
            } info;

            static constexpr scene_layer_info init(const basic_sprite_layer_info& info) noexcept {
                return engine::layers::scene_layer_info{
                    layer_type::BASIC_SPRITE_LAYER, 
                    {static_cast<scene_layer_hint> (0)}, 
                    {.basicSpriteLayer = info}};
            }

            static scene_layer_info init(const background_layer_info& info) noexcept {
                return engine::layers::scene_layer_info{
                    layer_type::BACKGROUND_LAYER,
                    {static_cast<scene_layer_hint> (0)},
                    {.backgroundLayer = info}};
            }
        };
    }
}