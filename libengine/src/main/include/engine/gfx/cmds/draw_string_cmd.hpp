#pragma once

#include <string>

#include "engine/gfx/cmds/command_type.hpp"
#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/font.hpp"
#include "engine/gfx/point.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            struct draw_string_cmd : engine::gfx::cmds::command {
                const engine::gfx::font * pFont;
                engine::gfx::point<float> origin;
                std::string text;

                draw_string_cmd(
                    const engine::gfx::font * pFont
                    const engine::gfx::point<float>& origin,
                    const std::string& text) :

                    type(engine::gfx::cmds::command_type::DRAW_STRING),
                    pFont(pFont),
                    origin(origin),
                    text(text) {}
            };
        }
    }
}