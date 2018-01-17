#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/matrix_order.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct rotate_transform_cmd : engine::gfx::cmds::command {
                float angle;
                engine::gfx::matrix_order order;

                rotate_transform_cmd(
                    float angle,
                    engine::gfx::matrix_order order) :
                    command(engine::gfx::cmds::command_type::ROTATE_TRANSFORM),
                    angle(angle),
                    order(order) {}
            };
        }
    }
}