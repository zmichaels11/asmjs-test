#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/image.hpp"
#include "engine/gfx/parallelogram.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {            
            struct draw_image_tint_cmd : public engine::gfx::cmds::command {
                const engine::gfx::image * pImage;
                engine::gfx::parallelogram<float> polygon;
                float r, g, b, a;

                draw_image_tint_cmd(
                    const engine::gfx::image * pImage,
                    const engine::gfx::parallelogram<float>& polygon,
                    float r, 
                    float g, 
                    float b, 
                    float a) :
                    command(engine::gfx::cmds::command_type::DRAW_IMAGE_TINT),
                    pImage(pImage),
                    polygon(polygon),
                    r(r),
                    g(g),
                    b(b),
                    a(a) {}
            };
        }
    }
}