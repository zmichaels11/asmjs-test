#include "graphics/depth_stencil_state_info.hpp"

#include <GLES3/gl3.h>

namespace graphics {
    void apply(const depth_stencil_state_info& info) {
        if (info.depthTestEnabled) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(static_cast<GLenum> (info.depthCompareOp));
        } else {
            glDisable(GL_DEPTH_TEST);
        }

        glDepthMask(info.depthWriteEnable);

        if (info.stencilTestEnable) {
            glEnable(GL_STENCIL_TEST);

            glStencilOpSeparate(GL_FRONT, static_cast<GLenum> (info.front.failOp), static_cast<GLenum> (info.front.depthFailOp), static_cast<GLenum> (info.front.passOp));
            glStencilOpSeparate(GL_BACK, static_cast<GLenum> (info.back.failOp), static_cast<GLenum> (info.back.depthFailOp), static_cast<GLenum> (info.back.passOp));

            glStencilMaskSeparate(GL_FRONT, info.front.writeMask);
            glStencilMaskSeparate(GL_BACK, info.back.writeMask);

            glStencilFuncSeparate(GL_FRONT, static_cast<GLenum> (info.front.compareOp), info.front.reference, info.front.compareMask);
            glStencilFuncSeparate(GL_BACK, static_cast<GLenum> (info.back.compareOp), info.back.reference, info.back.compareMask);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
    }

    template<>
    depth_stencil_state_info defaults() {
        return {
            false,
            compare_op::LESS,
            true,
            false,
            {stencil_op::KEEP, stencil_op::KEEP, stencil_op::KEEP, compare_op::ALWAYS, 0, 0xFFFF, 0xFFFF},
            {stencil_op::KEEP, stencil_op::KEEP, stencil_op::KEEP, compare_op::ALWAYS, 0, 0xFFFF, 0xFFFF}
        };
    }
}