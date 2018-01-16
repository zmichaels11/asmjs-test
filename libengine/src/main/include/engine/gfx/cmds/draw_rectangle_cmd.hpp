#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/pen.hpp"
#include "engine/gfx/point.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct draw_rectangle_cmd : engine::gfx::cmds::command {
                engine::gfx::pen pen;
                engine::gfx::point<float> topLeft, topRight, bottomLeft, bottomRight;

                draw_rectangle_cmd(
                    const engine::gfx::pen& pen,
                    const engine::gfx::point<float>& topLeft,
                    const engine::gfx::point<float>& topRight,
                    const engine::gfx::point<float>& bottomLeft,
                    const engine::gfx::point<float>& bottomRight) :

                    type(engine::gfx::cmds::command_type::DRAW_RECTANGLE),
                    pen(pen),
                    topLeft(topLeft),
                    topRight(topRight),
                    bottomLeft(bottomLeft),
                    bottomRight(bottomRight) {}
            };
        }
    }
}