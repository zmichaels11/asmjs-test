#include "engine/application.hpp"

#include "engine/gui/button.hpp"
#include "engine/gui/color.hpp"
#include "engine/gui/color_combo_box.hpp"
#include "engine/gui/component.hpp"
#include "engine/gui/dynamic_row_layout.hpp"
#include "engine/gui/frame.hpp"
#include "engine/gui/frame_opts.hpp"
#include "engine/gui/int_slider.hpp"
#include "engine/gui/label.hpp"
#include "engine/gui/option_group.hpp"
#include "engine/gui/static_row_layout.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/viewport_state_info.hpp"

#include <iostream>
#include <memory>
#include <vector>

struct AppData {
    std::shared_ptr<engine::gui::frame> root;
    engine::gui::color4ub background = {28, 48, 62, 255};

    engine::gui::option options[2] = {
        {"easy", nullptr},
        {"hard", nullptr}
    };

    graphics::clear_state_info clearState;
    graphics::viewport_state_info viewportState;
};

std::shared_ptr<AppData> _userData;

void frame(void * userData) {
    auto appData = reinterpret_cast<AppData *> (userData);

    graphics::apply(appData->viewportState);
    graphics::apply(appData->clearState);
}

void updateBackground() {
    _userData->clearState.color = {
        _userData->background.r / 255.0F,
        _userData->background.g / 255.0F,
        _userData->background.b / 255.0F,
        _userData->background.a / 255.0F};
}

int main(int argc, char** argv) {
    engine::application::init("GUI Test", 640, 480);

    _userData = std::make_shared<AppData>();    

    {
        _userData->root = std::make_shared<engine::gui::frame> (
            engine::gui::frame_opts::BORDER | engine::gui::frame_opts::MOVABLE | engine::gui::frame_opts::SCALABLE 
            | engine::gui::frame_opts::MINIMIZABLE | engine::gui::frame_opts::TITLE,
            50, 50, 230, 250,
            "Demo");

        std::vector<std::shared_ptr<engine::gui::component>> children;

        {            
            auto btn = std::make_shared<engine::gui::button>("Say Hello");

            btn->setOnAction([](auto p) {
                std::cout << "Hello from: " << p->getLabel() << std::endl;
            });

            children.push_back(std::make_shared<engine::gui::static_row_layout>(30, 80, 1));
            children.push_back(btn);
        }

        {            
            auto optMode = std::make_shared<engine::gui::option_group>(_userData->options, 2);

            optMode->setOnChange([](auto p) {
                std::cout << "Mode changed to: " << p->getSelected()->label << std::endl;
            });

            children.push_back(std::make_shared<engine::gui::dynamic_row_layout>(30, 2));
            children.push_back(optMode);
        }

        {
            auto slider = std::make_shared<engine::gui::int_slider>("Compression:", 0, 100, 10, 20);

            slider->setOnChange([](auto p) {
                std::cout << "Compression changed to: " << p->getValue() << std::endl;
            });

            children.push_back(std::make_shared<engine::gui::dynamic_row_layout>(25, 1));
            children.push_back(slider);
        }

        {
            children.push_back(std::make_shared<engine::gui::dynamic_row_layout>(20, 1));
            children.push_back(std::make_shared<engine::gui::label>("background:"));
            children.push_back(std::make_shared<engine::gui::dynamic_row_layout>(25, 1));

            auto picker = std::make_shared<engine::gui::color_combo_box>(_userData->background);

            picker->setOnChange([=](auto p) {
                _userData->background = picker->getColor();
                updateBackground();
            });

            children.push_back(picker);
        }

        _userData->root->setChildren(children);

        engine::application::setGUI(_userData->root.get(), 1);
    }

    updateBackground();
    _userData->clearState.buffers = graphics::clear_buffer::COLOR;	
    _userData->viewportState.x = 0;
    _userData->viewportState.y = 0;
    _userData->viewportState.width = 640;
    _userData->viewportState.height = 480;
    
    engine::application::setOnFrame(frame);
    engine::application::start(_userData);
}