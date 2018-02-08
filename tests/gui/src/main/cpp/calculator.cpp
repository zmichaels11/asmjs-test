#include "calculator.hpp"


#include <cstddef>
#include <cstdlib>

namespace gui_test {
    void calculator::handle(engine::layers::gui_layer * pLayer) noexcept {
        constexpr auto WINDOW_FLAGS = engine::layers::nuklear::panel_flags::BORDER
                | engine::layers::nuklear::panel_flags::NO_SCROLLBAR
                | engine::layers::nuklear::panel_flags::MOVABLE;

        if (pLayer->begin("Calculator", {10.0F, 10.0F, 180.0F, 250.0F}, WINDOW_FLAGS)) {
            {
                constexpr std::size_t MAX_BUFFER_SIZE = 256;

                char buffer[MAX_BUFFER_SIZE];
                auto len = std::snprintf(buffer, MAX_BUFFER_SIZE, "%.2f", *_current);

                pLayer->layoutRowDynamic(35, 1);
                pLayer->editString<engine::layers::nuklear::plugin_filter::FLOAT>(
                    engine::layers::nuklear::edit_types::SIMPLE,
                    buffer, &len, MAX_BUFFER_SIZE);

                buffer[len] = 0;
                *_current = std::atof(buffer);
            }

            pLayer->layoutRowDynamic(35, 4);
            
            bool solve = false;

            for (int i = 0; i < 16; i++) {
                if (i >= 12 && i < 15) {
                    if (i > 12) {
                        continue;
                    }

                    if (pLayer->buttonLabel("C")) {
                        _a = _b = _op = 0;
                        _current = &_a;
                        _set = false;
                    }

                    if (pLayer->buttonLabel("0")) {
                        *_current = *_current * 10.0F;
                        _set = false;
                    }

                    if (pLayer->buttonLabel("=")) {
                        solve = true;
                        _prev = _op;
                        _op = 0;
                    }
                } else if (((i + 1) % 4)) {
                    const char numbers[] = "789456123";
                    
                    auto numIdx = (i / 4) * 3 + i % 4;
                    if (pLayer->buttonText(&numbers[numIdx], 1)) {
                        *_current = *_current * 10.0F + (numbers[numIdx] - '0');
                        _set = false;
                    }
                } else {
                    const char ops[] = "+-*/";

                    if (pLayer->buttonText(&ops[i / 4], 1)) {
                        if (!_set) {
                            if (_current != &_b) {
                                _current = &_b;
                            } else {
                                _prev = _op;
                                solve = true;
                            }
                        }

                        _op = ops[i / 4];
                        _set = true;
                    }
                }
            }

            if (solve) {
                switch (_prev) {
                    case '+':
                        _a += _b;
                        break;
                    case '-':
                        _a -= _b;
                        break;
                    case '*':
                        _a *= _b;
                        break;
                    case '/':
                        _a /= _b;
                        break;
                    default:
                        break;
                }

                _current = &_a;

                if (_set) {
                    _current = &_b;
                }

                _b = 0.0;
                _set = false;
            }
        }

        pLayer->end();
    }
}