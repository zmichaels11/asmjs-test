#pragma once

#include "graphics/hinfo/input_assembly_state_info.hpp"
#include "graphics/hinfo/program_info.hpp"
#include "graphics/hinfo/vertex_array_info.hpp"

#include "graphics/hstate/blend_state_info.hpp"
#include "graphics/hstate/depth_stencil_state_info.hpp"
#include "graphics/hstate/rasterization_state_info.hpp"

namespace graphics {
    struct pipeline_info {
        program_info programState;
        vertex_array_info vertexInputState;
        input_assembly_state_info inputAssemblyState;
        rasterization_state_info rasterizationState;
        depth_stencil_state_info depthStencilState;
        blend_state_info blendState;
    };
}