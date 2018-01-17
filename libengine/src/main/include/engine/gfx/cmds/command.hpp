#pragma once

#include "engine/gfx/cmds/command_type.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct command {
                engine::gfx::cmds::command_type type;       
                
                command() {}

                command(engine::gfx::cmds::command_type type) :
                    type(type) {}
                    
                virtual ~command(){}         
            };
        }
    }
}