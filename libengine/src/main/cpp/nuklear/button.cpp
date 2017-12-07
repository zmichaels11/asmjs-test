#include "engine/gui/button.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {        
        namespace {
            nk_symbol_type _symbolType(symbol_type symbol);

            nk_color _color(const button_color_info& info);
        }

        namespace {
            nk_symbol_type _symbolType(symbol_type symbol) {
                switch (symbol) {
                    case symbol_type::X:
                        return NK_SYMBOL_X;
                    case symbol_type::UNDERSCORE:
                        return NK_SYMBOL_UNDERSCORE;
                    case symbol_type::CIRCLE_SOLID:
                        return NK_SYMBOL_CIRCLE_SOLID;
                    case symbol_type::CIRCLE_OUTLINE:
                        return NK_SYMBOL_CIRCLE_OUTLINE;
                    case symbol_type::RECT_SOLID:
                        return NK_SYMBOL_RECT_SOLID;
                    case symbol_type::RECT_OUTLINE:
                        return NK_SYMBOL_RECT_OUTLINE;
                    case symbol_type::TRIANGLE_UP:
                        return NK_SYMBOL_TRIANGLE_UP;
                    case symbol_type::TRIANGLE_DOWN:
                        return NK_SYMBOL_TRIANGLE_DOWN;
                    case symbol_type::TRIANGLE_LEFT:
                        return NK_SYMBOL_TRIANGLE_LEFT;
                    case symbol_type::TRIANGLE_RIGHT:
                        return NK_SYMBOL_TRIANGLE_RIGHT;
                    case symbol_type::PLUS:
                        return NK_SYMBOL_PLUS;
                    case symbol_type::MINUS:
                        return NK_SYMBOL_MINUS;
                    case symbol_type::MAX:
                        return NK_SYMBOL_MAX;
                    default:
                        return NK_SYMBOL_NONE;
                }
            }

            nk_color _color(const button_color_info& info) {
                return {info.r, info.g, info.b, info.a};
            }
        }

        template<>
        void button<button_color_info>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (ctx);

            if (nk_button_color(&nk->context, _color(_info))) {
                trigger();
            }
        }

        template<>
        void button<button_label_info>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (ctx);

            if (nk_button_label(&nk->context, _info.title.c_str())) {
                trigger();
            }
        }

        template<>
        void button<button_symbol_info>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (ctx);

            if (nk_button_symbol(&nk->context, _symbolType(_info.type))) {
                trigger();
            }
        }

        template<>
        void button<button_text_info>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (ctx);

            if (nk_button_text(&nk->context, _info.title, static_cast<int> (_info.length))) {
                trigger();
            }
        }
    }
}