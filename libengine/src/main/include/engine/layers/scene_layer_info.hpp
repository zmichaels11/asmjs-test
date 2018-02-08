#pragma once

#include "engine/layers/background_layer.hpp"
#include "engine/layers/basic_sprite_layer.hpp"
#include "engine/layers/bounds.hpp"
#include "engine/layers/clear_operation.hpp"
#include "engine/layers/gui_layer_info.hpp"
#include "engine/layers/layer_type.hpp"
#include "engine/layers/scene_layer_hint.hpp"

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
            
            union info_u {
                background_layer_info backgroundLayer;
                basic_sprite_layer_info basicSpriteLayer;                
                gui_layer_info guiLayer;

                constexpr info_u(const background_layer_info& info) noexcept:
                    backgroundLayer(info) {}

                constexpr info_u(const basic_sprite_layer_info& info) noexcept:
                    basicSpriteLayer(info) {}

                constexpr info_u(const gui_layer_info& info) noexcept:
                    guiLayer(info) {}

            } info;            

            constexpr scene_layer_info(const basic_sprite_layer_info& inf) noexcept :
                type(layer_type::BASIC_SPRITE_LAYER),
                ext({static_cast<scene_layer_hint> (0)}),
                info(inf) {}

            constexpr scene_layer_info(const background_layer_info& inf) noexcept :
                type(layer_type::BACKGROUND_LAYER),
                ext({static_cast<scene_layer_hint> (0)}),
                info(inf) {}

            constexpr scene_layer_info(const gui_layer_info& inf) noexcept :
                type(layer_type::GUI_LAYER),
                ext({static_cast<scene_layer_hint> (0)}),
                info(inf) {}
        };
    }
}