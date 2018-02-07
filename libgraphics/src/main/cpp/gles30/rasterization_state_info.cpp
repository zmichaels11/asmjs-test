#if defined(GLES30)
#include "pch.h"
#include "graphics/hstate/rasterization_state_info.hpp"

namespace graphics {
    template<>
    void apply(const rasterization_state_info& info) noexcept {
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
}
#endif
