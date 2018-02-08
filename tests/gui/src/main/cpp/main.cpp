#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <memory>
#include <vector>

#include "audio.hpp"
#include "graphics.hpp"
#include "engine.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

struct gui_test_data {
    struct calculator_t { 
        double a;
        double b;
        double * current;
        bool set;
        int prev;
        int op;

        calculator_t() {
            a = 0.0;
            b = 0.0;
            current = &a;
            set = false;
            prev = 0;
            op = 0;
        }

        void handle(engine::layers::gui_layer * pLayer) noexcept;
    } calculator;    
};

void gui_test_data::calculator_t::handle(engine::layers::gui_layer * pLayer) noexcept {
    constexpr auto WINDOW_FLAGS = engine::layers::nuklear::panel_flags::BORDER
            | engine::layers::nuklear::panel_flags::NO_SCROLLBAR
            | engine::layers::nuklear::panel_flags::MOVABLE;

    if (pLayer->begin("Calculator", {10.0F, 10.0F, 180.0F, 250.0F}, WINDOW_FLAGS)) {
        {
            constexpr std::size_t MAX_BUFFER_SIZE = 256;

            char buffer[MAX_BUFFER_SIZE];
            auto len = std::snprintf(buffer, MAX_BUFFER_SIZE, "%.2f", *current);

            pLayer->layoutRowDynamic(35, 1);
            pLayer->editString<engine::layers::nuklear::plugin_filter::FLOAT>(
                engine::layers::nuklear::edit_types::SIMPLE,
                buffer, &len, MAX_BUFFER_SIZE);

            buffer[len] = 0;
            *current = std::atof(buffer);
        }

        pLayer->layoutRowDynamic(35, 4);
        
        bool solve = false;

        for (int i = 0; i < 16; i++) {
            if (i >= 12 && i < 15) {
                if (i > 12) {
                    continue;
                }

                if (pLayer->buttonLabel("C")) {
                    a = b = op = 0;
                    current = &a;
                    set = false;
                }

                if (pLayer->buttonLabel("0")) {
                    *current = *current * 10.0F;
                    set = false;
                }

                if (pLayer->buttonLabel("=")) {
                    solve = true;
                    prev = op;
                    op = 0;
                }
            } else if (((i + 1) % 4)) {
                const char numbers[] = "789456123";
                
                auto numIdx = (i / 4) * 3 + i % 4;
                if (pLayer->buttonText(&numbers[numIdx], 1)) {
                    *current = *current * 10.0F + (numbers[numIdx] - '0');
                    set = false;
                }
            } else {
                const char ops[] = "+-*/";

                if (pLayer->buttonText(&ops[i / 4], 1)) {
                    if (!set) {
                        if (current != &b) {
                            current = &b;
                        } else {
                            prev = op;
                            solve = true;
                        }
                    }

                    op = ops[i / 4];
                    set = true;
                }
            }
        }

        if (solve) {
            switch (prev) {
                case '+':
                    a += b;
                    break;
                case '-':
                    a -= b;
                    break;
                case '*':
                    a *= b;
                    break;
                case '/':
                    a /= b;
                    break;
                default:
                    break;
            }

            current = &a;

            if (set) {
                current = &b;
            }

            b = 0.0;
            set = false;
        }
    }

    pLayer->end();
}

int main(int argc, char** argv) {
    engine::application::init({"GUI Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {1, 0}, HINTS});

    auto guiInfo = engine::layers::gui_layer_info {};
    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();
    auto guiLayerInfo = engine::layers::scene_layer_info(guiInfo);

    guiLayerInfo.ext.hints = engine::layers::scene_layer_hint::CLEAR;
    guiLayerInfo.ext.clear.type = engine::layers::clear_type::COLOR,
    guiLayerInfo.ext.clear.clearColor = engine::layers::color::rgb(20, 80, 120);

    pLayerInfos.push_back(guiLayerInfo);

    auto sceneInfo = engine::layers::scene_info {
        {nullptr, 0, nullptr, 0, nullptr, 0},
        pLayerInfos.data(), pLayerInfos.size()};    

    engine::application::setOnUpdate([](auto pUserData) {
        constexpr auto WINDOW_FLAGS = engine::layers::nuklear::panel_flags::BORDER
            | engine::layers::nuklear::panel_flags::NO_SCROLLBAR
            | engine::layers::nuklear::panel_flags::MOVABLE;

        auto pGuiTestData = reinterpret_cast<gui_test_data * > (pUserData);
        auto pScene = engine::application::getScene();
        auto pLayer = dynamic_cast<engine::layers::gui_layer *> (pScene->getLayer(0));

        pGuiTestData->calculator.handle(pLayer);
    });

    engine::application::setUserData(std::make_shared<gui_test_data>());

    engine::application::setScene(sceneInfo);

    engine::application::start();

    return 0;
}