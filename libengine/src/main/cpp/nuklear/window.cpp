#include "engine/gui/window.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void window::build(void * ctx) {
            this->_ctx = ctx;
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            if (nk_begin_titled(&nk->context, _info.name.c_str(), _info.title.c_str(), nk_rect(_info.bounds.x, _info.bounds.y, _info.bounds.w, _info.bounds.h), static_cast<nk_flags> (_info.flags))) {
                for (auto&& child : _children) {
                    child->build(ctx);
                }
            }

            nk_end(&nk->context);
        }

        void window::add(const std::shared_ptr<widget>& widget) {
            _children.push_back(widget);
        }

        void window::remove(const std::shared_ptr<widget>& widget) {
            _children.erase(std::remove(_children.begin(), _children.end(), widget), _children.end());
        }

        void window::addAll(const std::vector<std::shared_ptr<widget>>& widgets) {
            for (auto&& widget : widgets) {
                this->add(widget);
            }
        }

        void window::removeAll() {
            _children.clear();
        }

        const std::vector<std::shared_ptr<widget>>& window::getWidgets() const {
            return _children;
        }

        void window::show() {
            auto nk = reinterpret_cast<nk::nk_ctx *> (_ctx);

            nk_window_show(&nk->context, _info.name.c_str(), NK_SHOWN);
        }

        void window::hide() {
            auto nk = reinterpret_cast<nk::nk_ctx *> (_ctx);

            nk_window_show(&nk->context, _info.name.c_str(), NK_HIDDEN);
        }

        void window::setSize(float x, float y) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (_ctx);

            nk_window_set_size(&nk->context, _info.name.c_str(), nk_vec2(x, y));
        }

        void window::setPosition(float x, float y) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (_ctx);

            nk_window_set_position(&nk->context, _info.name.c_str(), nk_vec2(x, y));
        }
    }
}