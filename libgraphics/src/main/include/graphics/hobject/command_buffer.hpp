#pragma once

#include <vector>

#include "graphics/henum/pipeline_bind_point.hpp"

#include "graphics/hinfo/clear_info.hpp"
#include "graphics/hinfo/vertex_array_info.hpp"

#include "graphics/hobject/buffer.hpp"
#include "graphics/hobject/framebuffer.hpp"
#include "graphics/hobject/pipeline.hpp"
#include "graphics/hobject/renderpass.hpp"
#include "graphics/hobject/texture.hpp"

#include "graphics/hstate/scissor_state_info.hpp"
#include "graphics/hstate/viewport_state_info.hpp"

#include "graphics/tokens/state_machine.hpp"

namespace graphics {
    class command_buffer {
        std::vector<unsigned char> _bytecode;
        tokens::state_machine _current, _pending;

    public:        
        void executeCommands(
            const command_buffer * pCommandBuffers, 
            unsigned int count) noexcept;

        void bindPipeline(pipeline_bind_point bindpoint, const pipeline& pipeline) noexcept;

        void pushDescriptorSets() noexcept;

        void bindVertexBuffers() noexcept;

        void bindIndexBuffer() noexcept;        

        void setViewport(const viewport_state_info&) noexcept;

        void setScissor(const scissor_state_info& info) noexcept;

        //void setDepthBounds() noexcept;

        //void setLineWidth() noexcept;

        //void setDepthBias() noexcept;

        //void setStencilCompareMask() noexcept;

        //void setStencilWriteMask() noexcept;

        //void setStencilReference() noexcept;

        //void setBlendConstants() noexcept;

        //void drawIndirect() noexcept;

        //void drawIndexedIndirect() noexcept;

        //void dispatch() noexcept;

        //void dispatchIndirect() noexcept;        

        //void pushDescriptorSet() noexcept;

        //void pushConstants() noexcept;

        void draw(
            unsigned int vertexCount,
            unsigned int instanceCount,
            unsigned int firstVertex,
            unsigned int firstInstance) noexcept;        

        void drawIndexed(
            unsigned int indexCount,
            unsigned int instanceCount,
            unsigned int firstIndex,
            int vertexOffset,
            unsigned int firstInstance) noexcept;        

        void beginRenderpass(
            const renderpass& renderpass,
            const framebuffer& framebuffer,
            const clear_info * pClearValues,
            unsigned int nClearValues) noexcept;

        inline void endCommandBuffer() noexcept {
            _bytecode.shrink_to_fit();
        }
    };
}