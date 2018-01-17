#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/matrix_order.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct scale_transform_cmd : engine::gfx::cmds::command {
                float sx, sy;
                engine::gfx::matrix_order order;

                scale_transform_cmd(
                    float sx,
                    float sy,
                    engine::gfx::matrix_order order) :
                    command(engine::gfx::cmds::command_type::SCALE_TRANSFORM),
                    sx(sx),
                    sy(sy),
                    order(order) {}
            };
        }
    }
}