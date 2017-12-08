#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <memory>
#include <vector>

#include "engine/application.hpp"

#include "graphics/clear_state_info.hpp"
#include "graphics/viewport_state_info.hpp"

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

    graphics::clear_state_info clearState;
    graphics::viewport_state_info viewportState;
};

std::shared_ptr<AppData> _userData;

int main(int argc, char** argv) {
    engine::application::init("Calculator", 640, 480);

    auto userData = std::make_shared<AppData>();

    userData->clearState.buffers = graphics::clear_buffer::COLOR;
    userData->clearState.color = {0.1F, 0.25F, 0.4F, 1.0F};
    userData->viewportState.x = 0;
    userData->viewportState.y = 0;
    userData->viewportState.width = 640;
    userData->viewportState.height = 480;
    userData->calculator.current = &userData->calculator.a;

    engine::application::setOnUpdate([](auto userData) {
        auto appData = reinterpret_cast<AppData*> (userData);
        auto& calc = appData->calculator;

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
        auto appData = reinterpret_cast<AppData*> (userData);

        graphics::apply(appData->viewportState);
        graphics::apply(appData->clearState);
    });

    engine::application::start(userData);

    return 0;
}