#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/matrix_order.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct scale_transform : engine::gfx::cmds::command {
                float sx, sy;
                engine::gfx::matrix_order order;

                scale_transform(
                    float sx,
                    float sy,
                    engine::gfx::matrix_order order) :

                    type(engine::gfx::cmds::command_type::SCALE_TRANSFORM),
                    sx(sx),
                    sy(sy),
                    order(order) {}
            }
        }
    }
}