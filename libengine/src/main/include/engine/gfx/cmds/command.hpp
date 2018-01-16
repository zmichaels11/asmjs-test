#pragma once

namespace engine {
    namespace gfx {
        namespace cmds {
            struct command {
                engine::gfx::cmds::command_type type;       
                
                virtual ~command(){}         
            };
        }
    }
}