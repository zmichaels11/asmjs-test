#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct pop_matrix_cmd : public engine::gfx::cmds::command {
                pop_matrix_cmd() : command(engine::gfx::cmds::command_type::POP_MATRIX) {}
            };
        }
    }
}