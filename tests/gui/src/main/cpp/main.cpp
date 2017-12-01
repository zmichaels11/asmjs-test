#include "engine/application.hpp"
#include "engine/gui/button.hpp"
#include "engine/gui/component.hpp"
#include "engine/gui/dynamic_row_layout.hpp"
#include "engine/gui/frame.hpp"
#include "engine/gui/frame_opts.hpp"
#include "engine/gui/option_group.hpp"
#include "engine/gui/static_row_layout.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/viewport_state_info.hpp"

#include <iostream>
#include <memory>
#include <vector>

struct AppData {
    std::shared_ptr<engine::gui::frame> root;

    engine::gui::option options[2] = {
        {"easy", nullptr},
        {"hard", nullptr}
    };

    graphics::clear_state_info clearState;
    graphics::viewport_state_info viewportState;
};

void frame(void * userData) {
    auto appData = reinterpret_cast<AppData *> (userData);

    graphics::apply(appData->viewportState);
    graphics::apply(appData->clearState);
}

void sayHello(const engine::gui::button * btn) {
    std::cout << "Hello from <" << btn->getLabel() << ">" << std::endl;
}

void modeChange(const engine::gui::option_group * optGroup) {
    auto selected = optGroup->getSelected();
    
    std::cout << "Mode set to: " << selected->label << std::endl;
}

int main(int argc, char** argv) {
    engine::application::init("GUI Test", 640, 480);

    auto userData = std::make_shared<AppData>();    

    {
        userData->root = std::make_shared<engine::gui::frame> (
            engine::gui::frame_opts::BORDER | engine::gui::frame_opts::MOVABLE | engine::gui::frame_opts::SCALABLE 
            | engine::gui::frame_opts::MINIMIZABLE | engine::gui::frame_opts::TITLE,
            50, 50, 230, 250,
            "Demo");

        std::vector<std::shared_ptr<engine::gui::component>> children;

        {
            auto rowLayout = std::make_shared<engine::gui::static_row_layout>(30, 80, 1);
            auto btn = std::make_shared<engine::gui::button>("Say Hello");

            btn->setOnAction(sayHello);

            children.push_back(rowLayout);
            children.push_back(btn);
        }

        {
            auto rowLayout = std::make_shared<engine::gui::dynamic_row_layout>(30, 2);
            auto optMode = std::make_shared<engine::gui::option_group>(userData->options, 2);

            optMode->setOnChange(modeChange);

            children.push_back(rowLayout);
            children.push_back(optMode);
        }

        userData->root->setChildren(children);

        engine::application::setGUI(userData->root.get(), 1);            
    }


    userData->clearState.buffers = graphics::clear_buffer::COLOR;
	userData->clearState.color = {0.0F, 0.7F, 0.3F, 1.0F};
    userData->viewportState.x = 0;
    userData->viewportState.y = 0;
    userData->viewportState.width = 640;
    userData->viewportState.height = 480;
    
    engine::application::setOnFrame(frame);
    engine::application::start(userData);
}