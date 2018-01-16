#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/matrix_order.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct translate_transform_cmd : engine::gfx::cmds::command {
                float dx, dy;
                engine::gfx::matrix_order order;

                translate_transform_cmd(
                    float dx, float dy,
                    engine::gfx::matrix_order order) :

                    type(engine::gfx::cmds::command_type::TRANSLATE_TRANSFORM),
                    dx(dx),
                    dy(dy),
                    order(order) {}
            };
        }
    }
}