#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/color.hpp"
#include "engine/gfx/rect.hpp"
#include "engine/gfx/render_target.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct clear_color_cmd : public engine::gfx::cmds::command {
                engine::gfx::color color;

                clear_color_cmd(const engine::gfx::color& color) :
                    command(engine::gfx::cmds::command_type::CLEAR_COLOR),
                    color(color) {}
            };
        }
    }
}