#include "demo.hpp"

namespace gui_test {
    
    void demo::handle(engine::layers::gui_layer * pLayer) noexcept {
        constexpr auto WINDOW_FLAGS = engine::layers::nuklear::panel_flags::BORDER
            | engine::layers::nuklear::panel_flags::MOVABLE
            | engine::layers::nuklear::panel_flags::SCALABLE
            | engine::layers::nuklear::panel_flags::CLOSABLE
            | engine::layers::nuklear::panel_flags::MINIMIZABLE
            | engine::layers::nuklear::panel_flags::TITLE;

        if (pLayer->begin("Demo", {50, 50, 200, 200}, WINDOW_FLAGS)) {
            pLayer->layoutRowStatic(30, 80, 1);

            if (pLayer->buttonLabel("button")) {
                std::cout << "button pressed!" << std::endl;
            }

            pLayer->layoutRowDynamic(30, 2);
            
            if (pLayer->optionLabel("easy", _op == op_type::EASY)) {
                _op = op_type::EASY;
            }

            if (pLayer->optionLabel("hard", _op == op_type::HARD)) {
                _op = op_type::HARD;
            }

            pLayer->layoutRowDynamic(22, 1);
            pLayer->property<int>("Compression:", 0, &_property, 100, 10, 1.0F);
        }        

        pLayer->end();
    }
}