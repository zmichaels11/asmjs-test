#pragma once

#include <memory>

#include "renderer/color_transform.hpp"
#include "renderer/tree/list_node_info.hpp"
#include "renderer/tree/node.hpp"

namespace renderer {
    namespace tree {
        class list_node : public virtual node {
            struct resources_t {
                virtual ~resources_t() {}
            };

            std::unique_ptr<resources_t> _pResources;

            list_node(const list_node&) = delete;

            list_node& operator=(const list_node&) = delete;

        public:
            list_node(list_node&&) = default;

            list_node& operator=(list_node&&) = default;

            list_node(const renderer::tree::list_node_info& info) noexcept;

            virtual ~list_node() noexcept;

            virtual void render(void * pRenderTarget) noexcept;

            virtual void setColorTransform(const renderer::color_transform& colorTransform) noexcept;

            virtual bool supportsColorTransform() const noexcept;

            const renderer::tree::list_node_info& getInfo() const noexcept;
        };
    }
}