#include "engine/application.hpp"
#include "engine/gui/frame.hpp"
#include "engine/gui/frame_opts.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/viewport_state_info.hpp"

struct AppData {
    engine::gui::frame win;
    graphics::clear_state_info clearState;
    graphics::viewport_state_info viewportState;
};

void frame(void * userData) {
    auto appData = reinterpret_cast<AppData *> (userData);

    graphics::apply(appData->viewportState);
    graphics::apply(appData->clearState);
}

int main(int argc, char** argv) {
    engine::application::init("GUI Test", 640, 480);

    auto userData = std::make_shared<AppData>();

    userData->win = engine::gui::frame(
        engine::gui::frame_opts::BORDER
            | engine::gui::frame_opts::MOVABLE
            | engine::gui::frame_opts::SCALABLE
            | engine::gui::frame_opts::MINIMIZABLE
            | engine::gui::frame_opts::TITLE,
        50, 50, 230, 250,
        "Demo");

    userData->clearState.buffers = graphics::clear_buffer::COLOR;
	userData->clearState.color = {0.0F, 0.7F, 0.3F, 1.0F};
    userData->viewportState.x = 0;
    userData->viewportState.y = 0;
    userData->viewportState.width = 640;
    userData->viewportState.height = 480;

    engine::application::setGUI(&userData->win, 1);
    engine::application::setOnFrame(frame);
    engine::application::start(userData);
}