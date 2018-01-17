#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/image.hpp"
#include "engine/gfx/parallelogram.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {            
            struct draw_image_cmd : public engine::gfx::cmds::command {
                const engine::gfx::image * pImage;
                engine::gfx::parallelogram<float> polygon;

                draw_image_cmd(
                    const engine::gfx::image * pImage,
                    const engine::gfx::parallelogram<float>& polygon) :
                    command(engine::gfx::cmds::command_type::DRAW_IMAGE),
                    pImage(pImage),
                    polygon(polygon) {}
            };
        }
    }
}