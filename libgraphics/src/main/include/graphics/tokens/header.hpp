#pragma once

namespace graphics {
    namespace tokens {
        enum class header : unsigned int {            
            NOP = 0x0000,            
            USE_PROGRAM = 0x0001,
            BIND_TEXTURE = 0x0002,
            BIND_BUFFER_RANGE = 0x0003,

            // 
            PUSH_CONSTANT1I = 0x0011,
        };
    }
}
