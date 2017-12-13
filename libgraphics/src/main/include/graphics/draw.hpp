#pragma once

#include <cstddef>

#include "graphics/draw_mode.hpp"
#include "graphics/index_type.hpp"

namespace graphics {
    namespace draw {
        void arrays(graphics::draw_mode mode, int first, std::size_t count);

        void elements(graphics::draw_mode mode, std::size_t count, index_type type, void * offset);

        void arraysInstanced(graphics::draw_mode mode, int first, std::size_t count, std::size_t instanceCount);

        void elementsInstanced(graphics::draw_mode mode, std::size_t count, index_type type, void * offset, std::size_t instanceCount);

        void rangeElements(graphics::draw_mode mode, unsigned int start, unsigned int end, std::size_t count, index_type type, void * offset);
    }
}