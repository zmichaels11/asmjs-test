#pragma once

namespace graphics {
    namespace compute {
        void dispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY = 1, unsigned int numGroupsZ = 1) noexcept;
    }
}