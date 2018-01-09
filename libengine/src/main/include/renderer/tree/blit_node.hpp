#pragma once

#include <memory>

#include "renderer/tree/blit_node_info.hpp"
#include "renderer/tree/node.hpp"

namespace renderer {
    namespace tree {
        class blit_node : public virtual node {
            struct resources_t {
                virtual ~resources_t() {}
            };

            std::unique_ptr<resources_t> _pResources;

            blit_node(const blit_node&) = delete;

            blit_node& operator=(const blit_node&) = delete;

        public:
            blit_node(blit_node&&) = default;

            blit_node& operator=(blit_node&&) = default;

            blit_node(const renderer::tree::blit_node_info& info) noexcept;

            virtual ~blit_node() noexcept;

            virtual void render(void * pRenderTarget) noexcept;

            virtual void setColorTransform(const renderer::color_transform& colorTransform) noexcept;

            virtual bool supportsColorTransform() const noexcept;

            virtual bool isVisible() const noexcept;

            virtual void setVisible(bool isVisible) noexcept;

            const renderer::tree::blit_node_info& getInfo() const noexcept;
        };
    }
}