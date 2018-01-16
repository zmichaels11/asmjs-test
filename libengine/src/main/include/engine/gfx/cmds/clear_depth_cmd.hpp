#pragma once

#include "engine/gfx/cmds/command.hpp"
#include "engine/gfx/cmds/command_type.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct clear_depth_cmd : public engine::gfx::cmds::command {
                float depth;

                clear_depth_cmd(float depth) :
                    type(engine::gfx::cmds::command_type::CLEAR_DEPTH),
                    depth(depth) {}
            };
        }
    }
}