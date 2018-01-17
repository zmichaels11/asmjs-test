#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/composite_mode.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct set_composite_mode_cmd : public engine::gfx::cmds::command {
                engine::gfx::composite_mode compositeMode;

                set_composite_mode_cmd(engine::gfx::composite_mode compositeMode) :
                    command(engine::gfx::cmds::command_type::SET_COMPOSITE_MODE),
                    compositeMode(compositeMode) {}
            };
        }
    }
}