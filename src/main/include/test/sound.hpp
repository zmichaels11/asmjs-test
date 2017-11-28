#pragma once

#include <memory>

#include "audio/sound.hpp"
#include "graphics/clear_state_info.hpp"

namespace test {
    namespace sound {
        struct AppData {
            std::unique_ptr<audio::sound> sound;
            graphics::clear_state_info clearState;
        };

        void frame(void * pUserData);

        std::shared_ptr<void> init();
    }
}