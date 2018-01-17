#include "engine/gfx/cmds/set_transform_cmd.hpp"

#include <cstring>

#include "engine/gfx/cmds/command_type.hpp"

namespace engine {
    namespace gfx {
        namespace cmds {
            set_transform_cmd::set_transform_cmd(const float * matrix) {
                type = engine::gfx::cmds::command_type::SET_TRANSFORM;

                std::memcpy(this->matrix, matrix, sizeof(float) * 16);
            }
        }
    }
}