#include "renderer/gui_layer.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "renderer/scissor_rect.hpp"

#include "nk/nk_ctx.hpp"

namespace renderer {
    namespace {
        void _onError(const std::string& msg) {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }
    }
    void gui_layer::update() {
        auto ctx = nk::getContext();

        ctx->newFrame();
    }

    void gui_layer::doFrame() {
        auto ctx = nk::getContext();

        ctx->render();
    }

    void gui_layer::setProjection(const float * proj) {
        _onError("gui_layer does not support projection matrix!");
    }

    void gui_layer::setScissor(const scissor_rect& scissor) {
        _onError("gui_layer does not support scissor_rect!");
    }

    const scissor_rect& gui_layer::getScissor() const {
        _onError("gui_layer does not support scissor_rect!");
    }
}