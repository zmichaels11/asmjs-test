#include "engine/application.hpp"

#include "engine/gui/button.hpp"
#include "engine/gui/color.hpp"
#include "engine/gui/combobox.hpp"
#include "engine/gui/widget.hpp"
#include "engine/gui/dynamic_row_layout.hpp"
#include "engine/gui/window.hpp"
#include "engine/gui/window_info.hpp"
#include "engine/gui/slider.hpp"
#include "engine/gui/label.hpp"
#include "engine/gui/option_group.hpp"
#include "engine/gui/static_row_layout.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/viewport_state_info.hpp"

#include <iostream>
#include <memory>
#include <vector>

struct AppData {
    engine::gui::color4ub background = {28, 48, 62, 255};

    engine::gui::option options[2] = {
        {"easy", nullptr},
        {"hard", nullptr}
    };

    graphics::clear_state_info clearState;
    graphics::viewport_state_info viewportState;
};

void updateBackground();

std::shared_ptr<AppData> _userData;

int main(int argc, char** argv) {
    engine::application::init("GUI Test", 640, 480);

    _userData = std::make_shared<AppData>();    

    {
        auto root = std::make_shared<engine::gui::window> (
            engine::gui::window_info {
                "Demo", "hDemo",
                engine::gui::window_flag::BORDER | engine::gui::window_flag::MOVABLE | engine::gui::window_flag::SCALABLE 
                    | engine::gui::window_flag::MINIMIZABLE | engine::gui::window_flag::TITLE,
            {50, 50, 230, 250}});

        {            
            auto btn = engine::gui::createButton(engine::gui::button_label_info{"Say Hello"});

            btn->setOnAction([](auto p) {
                std::cout << "Hello!" << std::endl;
            });

            root->add(std::make_shared<engine::gui::static_row_layout>(30, 80, 1));
            root->add(btn);
        }

        {            
            auto optMode = std::make_shared<engine::gui::option_group>(_userData->options, 2);

            optMode->setOnChange([](auto p) {
                std::cout << "Mode changed to: " << p->getSelected()->label << std::endl;
            });

            root->add(std::make_shared<engine::gui::dynamic_row_layout>(30, 2));
            root->add(optMode);
        }

        {
            auto slider = std::make_shared<engine::gui::slider<float>>(0.0F, 1.0F, 0.1F, 0.5F);

            slider->setOnChange([](auto p) {
                std::cout << "Compression changed to: " << p->getValue() << std::endl;
            });

            root->add(std::make_shared<engine::gui::dynamic_row_layout>(25, 1));
            root->add(std::make_shared<engine::gui::label>("Compression:"));            
            root->add(slider);
        }

        {
            root->add(std::make_shared<engine::gui::dynamic_row_layout>(20, 1));
            root->add(std::make_shared<engine::gui::label>("background:"));
            root->add(std::make_shared<engine::gui::dynamic_row_layout>(25, 1));

            auto picker = std::make_shared<engine::gui::color_combobox>(_userData->background);

            picker->setOnChange([=](auto p) {
                _userData->background = picker->getColor();
                updateBackground();
            });

            root->add(picker);
        }

        engine::application::add(root);
    }

    updateBackground();
    _userData->clearState.buffers = graphics::clear_buffer::COLOR;	
    _userData->viewportState.x = 0;
    _userData->viewportState.y = 0;
    _userData->viewportState.width = 640;
    _userData->viewportState.height = 480;
    
    engine::application::setOnFrame([](auto userData){
        auto appData = reinterpret_cast<AppData *> (userData);

        graphics::apply(appData->viewportState);
        graphics::apply(appData->clearState);
    });

    engine::application::start(_userData);
}

void updateBackground() {
    _userData->clearState.color = {
        _userData->background.r / 255.0F,
        _userData->background.g / 255.0F,
        _userData->background.b / 255.0F,
        _userData->background.a / 255.0F};
}