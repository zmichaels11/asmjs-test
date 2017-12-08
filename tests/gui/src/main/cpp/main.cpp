#include "engine/application.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/viewport_state_info.hpp"

#include "nk/button.hpp"
#include "nk/color.hpp"
#include "nk/color_picker.hpp"
#include "nk/combobox.hpp"
#include "nk/label.hpp"
#include "nk/layout.hpp"
#include "nk/property.hpp"
#include "nk/radio.hpp"
#include "nk/widget.hpp"
#include "nk/window.hpp"

#include <iostream>
#include <memory>
#include <vector>

enum class setting {EASY, HARD};

struct AppData {
    nk::color color{28, 48, 62, 255};
    setting mode;
    int property;
    graphics::clear_state_info clearState;
    graphics::viewport_state_info viewportState;
};

int main(int argc, char** argv) {
    engine::application::init("GUI Test", 640, 480);

    auto userData = std::make_shared<AppData>();
    
    userData->clearState.buffers = graphics::clear_buffer::COLOR;	    
    userData->clearState.color = {0.1F, 0.25F, 0.4F, 1.0F};
    userData->viewportState.x = 0;
    userData->viewportState.y = 0;
    userData->viewportState.width = 640;
    userData->viewportState.height = 480;
    userData->mode = setting::EASY;
    
    engine::application::setOnUpdate([](auto userData) {
        auto appData = reinterpret_cast<AppData *> (userData);
                
        nk::window window({"Demo", "demo", {50, 50, 300, 250}, nk::window_flag::BORDER | nk::window_flag::MOVABLE | nk::window_flag::SCALABLE | nk::window_flag::MINIMIZABLE | nk::window_flag::TITLE});
        
        if (window) {
            nk::layout::row_static(30, 80, 1);

            if (nk::button_label("button")) {
                std::cout << "button pressed" << std::endl;
            }

            nk::layout::row_dynamic(30, 2);

            if (nk::radio_label("easy", appData->mode == setting::EASY)) {
                appData->mode = setting::EASY;
            }

            if (nk::radio_label("hard", appData->mode == setting::HARD)) {
                appData->mode = setting::HARD;
            }

            nk::layout::row_dynamic(20, 1);
            nk::property("Compression:", 0, &appData->property, 100, 10);

            nk::layout::row_dynamic(20, 1);
            nk::label("backgrounds:");

            nk::layout::row_dynamic(25, 1);

            nk::combobox_color combobox({appData->color, {nk::widget::width(), 400}});

            if (combobox) {
                nk::layout::row_dynamic(120, 1);
                appData->color = nk::color_picker(appData->color);
                nk::layout::row_dynamic(25, 1);
                appData->color.r = static_cast<std::uint8_t> (nk::property("#R:", 0, appData->color.r, 255, 1));
                appData->color.g = static_cast<std::uint8_t> (nk::property("#G:", 0, appData->color.g, 255, 1));
                appData->color.b = static_cast<std::uint8_t> (nk::property("#B:", 0, appData->color.b, 255, 1));
                appData->color.a = static_cast<std::uint8_t> (nk::property("#A:", 0, appData->color.a, 255, 1));
            }
        }

        appData->clearState.color = {appData->color.r / 255.0F, appData->color.g / 255.0F, appData->color.b / 255.0F, appData->color.a / 255.0F};
    });

    engine::application::setOnFrame([](auto userData){         
        auto appData = reinterpret_cast<AppData *> (userData);

        graphics::apply(appData->viewportState);
        graphics::apply(appData->clearState);
    });

    engine::application::start(userData);
}