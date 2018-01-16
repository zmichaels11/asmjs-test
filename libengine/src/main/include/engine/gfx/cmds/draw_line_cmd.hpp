#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/pen.hpp"
#include "engine/gfx/point.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct draw_line_cmd : public engine::gfx::cmds::command {
                engine::gfx::pen pen;
                engine::gfx::point<float> start, end;

                draw_line_cmd(
                    const engine::gfx::pen& pen,
                    const engine::gfx::point<float>& start,
                    const engine::gfx::point<float>& end) :

                    type(engine::gfx::cmds::command_type::DRAW_LINE),
                    pen(pen),
                    start(start),
                    end(end) {}
            };
        }
    }
}