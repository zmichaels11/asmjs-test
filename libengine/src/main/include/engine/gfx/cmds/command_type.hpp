#pragma once

namespace engine {
    namespace gfx {
        namespace cmds {
            enum class command_type {
                CLEAR_COLOR,
                CLEAR_CLIP,
                CLEAR_DEPTH,
                DRAW_IMAGE,
                DRAW_IMAGE_COLOR_TRANSFORM,
                DRAW_IMAGE_BC,
                DRAW_IMAGE_HSV,
                DRAW_IMAGE_TINT,
                DRAW_LINE,
                DRAW_RECTANGLE,
                DRAW_STRING,
                ROTATE_TRANSFORM,
                SCALE_TRANSFORM,
                SET_TRANSFORM,
                TRANSLATE_TRANSFORM,
                SAVE,
                RESTORE,
                SET_COMPOSITE_MODE
            };
        }
    }
}