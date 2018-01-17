#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/color_transform.hpp"
#include "engine/gfx/image.hpp"
#include "engine/gfx/parallelogram.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {            
            struct draw_image_color_transform_cmd : public engine::gfx::cmds::command {
                const engine::gfx::image * pImage;
                engine::gfx::parallelogram<float> polygon;
                engine::gfx::color_transform colorTransform;

                draw_image_color_transform_cmd(
                    const engine::gfx::image * pImage,
                    const engine::gfx::parallelogram<float>& polygon,
                    const engine::gfx::color_transform& colorTransform) :
                    command(engine::gfx::cmds::command_type::DRAW_IMAGE_COLOR_TRANSFORM),
                    pImage(pImage),
                    polygon(polygon),
                    colorTransform(colorTransform) {}
            };
        }
    }
}