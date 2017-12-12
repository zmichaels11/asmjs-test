#include "renderer/gui_layer.hpp"

#include <iostream>
#include <memory>

#include "nk/nk_ctx.hpp"

namespace renderer {
    void gui_layer::update(double timestep) {
        auto ctx = nk::getContext();

        ctx->newFrame();
    }

    void gui_layer::doFrame(double timestep) {
        auto ctx = nk::getContext();

        ctx->render();
    }

    void gui_layer::setProjection(const float * proj) {
        std::cerr << "Gui Layer does not support projection matrix!" << std::endl;
        __builtin_trap();
    }
}