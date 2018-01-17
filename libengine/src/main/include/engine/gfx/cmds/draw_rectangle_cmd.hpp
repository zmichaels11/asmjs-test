#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/pen.hpp"
#include "engine/gfx/rect.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct draw_rectangle_cmd : engine::gfx::cmds::command {
                engine::gfx::pen pen;
                engine::gfx::rect<float, float> area;

                draw_rectangle_cmd(
                    const engine::gfx::pen& pen,
                    const engine::gfx::rect<float, float>& area) :
                    command(engine::gfx::cmds::command_type::DRAW_RECTANGLE),
                    pen(pen),
                    area(area) {}
            };
        }
    }
}