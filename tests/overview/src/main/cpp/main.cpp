#include <iostream>
#include <memory>
#include <vector>

#include "engine/application.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/viewport_state_info.hpp"

#include "nk/window_flag.hpp"

struct AppData {
    struct overview_t {
        bool showMenu;
        bool titleBar;
        bool border;
        bool resize;
        bool noScrollbar;
        bool movable;
        bool scaleLeft;
        
    } overview;

    graphics::clear_state_info clearState;
    graphics::viewport_state_info viewportState;
};

int main(int argc, char** argv) {
    engine::application::init("Nuklear Overview", 640, 480);

    auto userData = std::make_shared<AppData>();

    userData->clearState.buffers = graphics::clear_buffer::COLOR;
    userData->clearState.color = {0.1F, 0.25F, 0.4F, 1.0F};
    userData->viewportState.x = 0;
    userData->viewportState.y = 0;
    userData->viewportState.width = 640;
    userData->viewportState.height = 480;

    engine::application::setOnUpdate([](auto userData) {
        auto appData = reinterpret_cast<AppData*> (userData);
        auto& overview = appData->overview;

        nk::window_flag windowFlags = static_cast<nk::window_flag> (0);

        if (overview.border) {
            windowFlags |= nk::window_flag::BORDER;
        }
    });

    engine::application::setOnFrame([](auto userData) {
        auto appData = reinterpret_cast<AppData*> (userData);

        graphics::apply(appData->viewportState);
        graphics::apply(appData->clearState);        
    });

    engine::application::start(userData);

    return 0;
}