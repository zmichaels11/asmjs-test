#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/image.hpp"
#include "engine/gfx/parallelogram.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {            
            struct draw_image_hsv_cmd : public engine::gfx::cmds::command {
                const engine::gfx::image * pImage;
                engine::gfx::parallelogram<float> points;
                float hue, saturation, value;

                draw_image_hsv_cmd(
                    const engine::gfx::image * pImage,
                    const engine::gfx::parallelogram<float>& polygon,
                    float hue,
                    float saturation,
                    float value) :
                    command(engine::gfx::cmds::command_type::DRAW_IMAGE_HSV),
                    pImage(pImage),
                    hue(hue),
                    saturation(saturation),
                    value(value) {}
            };
        }
    }
}