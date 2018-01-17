#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct set_transform_cmd : public engine::gfx::cmds::command {
                float matrix[16];

                set_transform_cmd(const float * matrix);
            };
        }
    }
}