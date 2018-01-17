#pragma once

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/image.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct draw_tile_cmd : public engine::gfx::cmds::command {
                const engine::gfx::image * pImage;
                int x, y;

                draw_tile_cmd(
                    const engine::gfx::image * pImage,
                    int x, int y) :
                    command(engine::gfx::cmds::command_type::DRAW_TILE),
                    pImage(pImage),
                    x(x),
                    y(y) {}
            };
        }
    }
}