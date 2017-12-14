#include "graphics/rasterization_state_info.hpp"

#include <GLES3/gl3.h>

#include "graphics/cull_mode.hpp"

namespace graphics {
    void apply(const rasterization_state_info& info) {
        glLineWidth(info.lineWidth);
        
        if (info.polygonOffset.enabled) {
            glPolygonOffset(info.polygonOffset.factor, info.polygonOffset.units);
        }

        //glPolygonMode(static_cast<GLenum> (info.polygonMode));

        if (info.cullMode != cull_mode::NONE) {
            glEnable(GL_CULL_FACE);
            glCullFace(static_cast<GLenum> (info.cullMode));
            glFrontFace(static_cast<GLenum> (info.frontFace));
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    template<>
    rasterization_state_info defaults() {
        return {
            front_face::CCW,
            1.0F,
            cull_mode::BACK,
            {false, 0.0F, 0.0F},
            polygon_mode::FILL
        };
    }
}