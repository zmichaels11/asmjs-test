#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/combine_mode.hpp"
#include "engine/gfx/rect.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct clip_rect_cmd : public engine::gfx::cmds::command {
                engine::gfx::combine_mode mode;
                engine::gfx::rect<int, unsigned int> area;

                clip_rect_cmd(
                    engine::gfx::combine_mode mode,
                    engine::gfx::rect<int, unsigned int> area) :
                    command(engine::gfx::cmds::command_type::CLIP_RECT),
                    mode(mode),
                    area(area) {}
            };
        }
    }
}