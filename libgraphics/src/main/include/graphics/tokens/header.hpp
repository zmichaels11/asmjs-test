#pragma once

namespace graphics {
    namespace tokens {
        enum class header : unsigned int {            
            NOP = 0x0000,            
            BIND_PIPELINE = 0x0001,
            PUSH_DESCRIPTOR_SETS = 0x0002,
            BIND_VERTEX_BUFFERS = 0x0003,
            BIND_INDEX_BUFFER = 0x0004,
            SET_VIEWPORT = 0x0005,
            SET_SCISSOR = 0x0006,
            SET_DEPTH_BOUNDS = 0x0007,
            SET_LINE_WIDTH = 0x0008,
            SET_STENCIL_COMPARE_MASK = 0x0009,
            SET_STENCIL_WRITE_MASK = 0x000A,
            SET_STENCIL_REFERENCE = 0x000B,
            SET_BLEND_CONSTANTS = 0x000C,
            DRAW = 0x000D,
            DRAW_INDEXED = 0x000E,
            DRAW_INDIRECT = 0x000F,
            DRAW_INDEXED_INDIRECT = 0x0010,
            DISPATCH = 0x0011,
            DISPATCH_INDIRECT = 0x0012,
            PUSH_CONSTANTS = 0x0013,
            // emulates an extension, so lets give it some room.
            PUSH_DESCRIPTOR_SET = 0x1000
        }
    }
}
