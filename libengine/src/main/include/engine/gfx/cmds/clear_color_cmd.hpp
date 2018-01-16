#pragma once

#include "engine/gfx/cmds/command.hpp"
#include "engine/gfx/cmds/command_type.hpp"

#include "engine/gfx/color.hpp"
#include "engine/gfx/rect.hpp"
#include "engine/gfx/render_target.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct clear_color_cmd : public engine::gfx::cmds::command {
                engine::gfx::rect<int, unsigned int> bounds;                
                engine::gfx::color color;

                clear_color_cmd(
                    const engine::gfx::rect<int, unsigned int>& bounds,
                    const engine::gfx::color& color) :

                    type(engine::gfx::cmds::command_type::CLEAR_COLOR),
                    bounds(bounds),
                    color(color) {}
            };
        }
    }
}