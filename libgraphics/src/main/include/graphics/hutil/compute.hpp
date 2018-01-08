#pragma once

namespace graphics {
    class compute {
        compute() = delete;

    public:
        static void dispatch(unsigned int numGroupsX, unsigned int numGroupsY = 1, unsigned int numGroupsZ = 1) noexcept;
    };
}