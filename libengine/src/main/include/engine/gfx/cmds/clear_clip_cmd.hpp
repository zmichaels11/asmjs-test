#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/rect.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct clear_clip_cmd : public engine::gfx::cmds::command {
                engine::gfx::rect<int, unsigned int> bounds;

                clear_clip_cmd(const engine::gfx::rect<int, unsigned int>& bounds) :                    
                    command(engine::gfx::cmds::command_type::CLEAR_CLIP),
                    bounds(bounds) {}
            };
        }
    }
}