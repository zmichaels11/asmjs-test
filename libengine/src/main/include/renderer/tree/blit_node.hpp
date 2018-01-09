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

            blit_node(const blit_node_info& info) noexcept;

            virtual ~blit_node() noexcept;

            virtual void setProjection(const float * projection) noexcept;
        };
    }
}