#pragma once

#include <cstddef>

#include "graphics/draw_mode.hpp"
#include "graphics/index_type.hpp"

namespace graphics {
    namespace draw {
        void drawArrays(graphics::draw_mode mode, int first, std::size_t count);

        void drawElements(graphics::draw_mode mode, std::size_t count, index_type type, void * offset);

        void drawArraysInstanced(graphics::draw_mode mode, int first, std::size_t count, std::size_t instanceCount);

        void drawElementsInstanced(graphics::draw_mode mode, std::size_t count, index_type type, void * offset, std::size_t instanceCount);

        void drawRangeElements(graphics::draw_mode mode, unsigned int start, unsigned int end, std::size_t count, index_type type, void * offset);
    }
}