#pragma once

#include <cstddef>

#include "graphics/draw_mode.hpp"
#include "graphics/index_type.hpp"

namespace graphics {
    class draw {
        draw() = delete;

    public:
        static void arrays(graphics::draw_mode mode, int first, std::size_t count) noexcept;

        static void elements(graphics::draw_mode mode, std::size_t count, index_type type, void * offset) noexcept;

        static void arraysInstanced(graphics::draw_mode mode, int first, std::size_t count, std::size_t instanceCount) noexcept;

        static void elementsInstanced(graphics::draw_mode mode, std::size_t count, index_type type, void * offset, std::size_t instanceCount) noexcept;

        static void rangeElements(graphics::draw_mode mode, unsigned int start, unsigned int end, std::size_t count, index_type type, void * offset) noexcept;
    };
}