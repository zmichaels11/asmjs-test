#include "graphics/hobject/command_buffer.hpp"

#include <cstring>

#include <vector>

#include "graphics/henum/pipeline_bind_point.hpp"

#include "graphics/hutil/uniform.hpp"

namespace graphics {
    namespace {
        void _encode(
            std::vector<unsigned char>& out,
            const void * pData,
            std::size_t size) noexcept;

        void _flush(
            std::vector<unsigned char>& out,
            tokens::state_machine& current,
            const tokens::state_machine& pending) noexcept;
    }

    void command_buffer::executeCommands(
        const command_buffer * pCommandBuffers, 
        unsigned int count) noexcept {

        auto totalSize = _bytecode.size();

        for (decltype(count) i = 0; i < count; i++) {
            totalSize += pCommandBuffers[i]._bytecode.size();   
        }

        auto cat = std::vector<unsigned char>();

        cat.reserve(totalSize);
        cat.insert(cat.end(), _bytecode.begin(), _bytecode.end());

        for (decltype(count) i = 0; i < count; i++) {
            auto& cur = pCommandBuffers[i]._bytecode;

            cat.insert(cat.end(), cur.begin(), cur.end());
        }

        _bytecode = cat;
    }

    void command_buffer::pushDescriptorSets() noexcept {}

    void command_buffer::bindVertexBuffers() noexcept {}

    void command_buffer::bindIndexBuffer() noexcept {}

    void command_buffer::bindPipeline(
        pipeline_bind_point bindpoint,
        const pipeline& pipeline) noexcept {        
        
    }

    void command_buffer::setViewport(const viewport_state_info& viewport) noexcept {
        
    }

    void command_buffer::setScissor(const scissor_state_info& scissor) noexcept {
        
    }

    void command_buffer::beginRenderpass(
        const renderpass& renderpass,
        const framebuffer& framebuffer,
        const clear_info * pClearValues,
        unsigned int nClearValues) noexcept {

    }

    namespace {
        void _encode(
            std::vector<unsigned char>& out,
            const void * pData,
            std::size_t size) noexcept {

            auto pTmp = reinterpret_cast<const unsigned char * > (pData);

            out.insert(out.end(), pTmp, pTmp + size);
        }

        void _flush(
            std::vector<unsigned char>& out,
            tokens::state_machine& current,
            const tokens::state_machine& pending) noexcept {
            
        }
    }
}