#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <memory>
#include <vector>

#include "engine/application.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/imageio.hpp"
#include "graphics/viewport_state_info.hpp"

#include "math/mat4.hpp"

#include "renderer/image.hpp"
#include "renderer/image_layer_info.hpp"
#include "renderer/layer_type.hpp"
#include "renderer/scene.hpp"
#include "renderer/scene_layer_info.hpp"
#include "renderer/text_layer.hpp"
#include "renderer/text_layer_info.hpp"

#include "nk/button.hpp"
#include "nk/edit.hpp"
#include "nk/layout.hpp"
#include "nk/window.hpp"

struct AppData {
    struct calculator_t {
        int prev, op;
        bool set;
        double a, b;
        double * current;        
    } calculator;
};

std::shared_ptr<AppData> _userData;

int main(int argc, char** argv) {
    engine::application::init("Calculator", 640, 480);    

    auto sceneInfo = renderer::scene_info();
    auto backgroundImgRes = graphics::image_io::read("data/images/environment.png", 4);    
    auto backgroundInfo = renderer::image_layer_info{{
        {backgroundImgRes->getWidth(), backgroundImgRes->getHeight(), renderer::image_format::R8G8B8A8_UNORM, backgroundImgRes->getData()},
        renderer::image_filter::LINEAR, renderer::image_filter::LINEAR,
        renderer::image_scroll_type::STATIC, renderer::image_scroll_type::STATIC,
        nullptr}, 
        {}};

    auto textLayerInfo = renderer::defaults<renderer::text_layer_info>();

    renderer::scene_layer_info layerInfos[] {
        {renderer::layer_type::IMAGE, &backgroundInfo},        
        {renderer::layer_type::GUI, nullptr},
        {renderer::layer_type::TEXT, &textLayerInfo},
    };

    engine::application::setScene({layerInfos, 3, {{0.1F, 0.25F, 0.4F, 1.0F}, 1.0F}});

    auto userData = std::make_shared<AppData>();
    
    userData->calculator.current = &userData->calculator.a;

    engine::application::setOnUpdate([](auto userData) {
        auto appData = reinterpret_cast<AppData*> (userData);
        auto& calc = appData->calculator;
        auto pScene = engine::application::getScene();
        auto pTextLayer = dynamic_cast<renderer::text_layer*>(pScene->getLayer(2));

        pTextLayer->text({"Hello World!", {1.0F, 0.0F, 1.0F, 1.0F}, 256, 256});

        auto mproj = math::mat4::ortho(0, 640, 480, 0, 0, 1);
        float proj[16];

        mproj.data(proj);

        pTextLayer->setProjection(proj);        

        std::size_t i = 0;
        int solve = 0;

        nk::window window({"Calculator", "calc", {10, 10, 180, 250}, nk::window_flag::BORDER | nk::window_flag::NO_SCROLLBAR | nk::window_flag::MOVABLE});

        if (window) {                        
            nk::layout::row_dynamic(35, 1);

            char buffer[256];
            int len = std::snprintf(buffer, 256, "%.2f", *calc.current);

            nk::edit_string(nk::edit_type::SIMPLE, buffer, &len, 255, nk::edit_filter::FLOAT);

            buffer[len] = 0;

            *calc.current = std::atof(buffer);        

            nk::layout::row_dynamic(35, 4);

            bool solve = false;
            static const char OPS[] = "+-*/";

            for (int i = 0; i < 16; i++) {
                if (i >= 12 && i < 15) {
                    if (i > 12) {
                        continue;
                    }

                    if (nk::button_label("C")) {
                        calc.a = calc.b = calc.op = 0;
                        calc.current = &calc.a;
                        calc.set = false;
                    }

                    if (nk::button_label("0")) {
                        *calc.current = *calc.current * 10.0F;
                        calc.set = 0;
                    }

                    if (nk::button_label("=")) {
                        solve = true;
                        calc.prev = calc.op;
                        calc.op = 0;
                    }                
                } else if (((i+1) % 4)) {
                    static const char NUMBERS[] = "789456123";

                    if (nk::button_text(&NUMBERS[(i / 4) * 3 + i % 4], 1)) {
                        *calc.current = *calc.current * 10.0F + NUMBERS[(i / 4) * 3 + i % 4] - '0';
                        calc.set = false;
                    }
                } else if (nk::button_text(&OPS[i / 4], 1)) {
                    if (!calc.set) {
                        if (calc.current != &calc.b) {
                            calc.current = &calc.b;
                        } else {
                            calc.prev = calc.op;
                            solve = true;
                        }
                    }

                    calc.op = OPS[i / 4];
                    calc.set = true;
                }
            }

            if (solve) {
                if (calc.prev == '+') {
                    calc.a = calc.a + calc.b;
                } else if (calc.prev == '-') {
                    calc.a = calc.a - calc.b;
                } else if (calc.prev == '*') {
                    calc.a = calc.a * calc.b;
                } else if (calc.prev == '/') {
                    calc.a = calc.a / calc.b;
                }

                calc.current = &calc.a;

                if (calc.set) {
                    calc.current = &calc.b;
                }

                calc.b = 0;
                calc.set = false;
            }
        }
    });

    engine::application::setOnFrame([](auto userData) {
        
    });

    engine::application::start(userData);

    return 0;
}