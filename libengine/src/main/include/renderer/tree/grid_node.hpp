#pragma once

#include <memory>

#include "renderer/tree/grid_node_info.hpp"
#include "renderer/tree/grid_node_leaf.hpp"
#include "renderer/tree/node.hpp"

namespace renderer {
    namespace tree {
        class grid_node : public virtual node {
            struct resources_t {
                virtual ~resources_t() {}
            };

            std::unique_ptr<resources_t> _pResources;

            grid_node(const grid_node&) = delete;

            grid_node& operator=(const grid_node&) = delete;

        public:
            grid_node(grid_node&&) = default;

            grid_node& operator=(grid_node&&) = default;

            grid_node(const grid_node_info& info) noexcept;

            virtual ~grid_node() noexcept;

            virtual void setProjection(const float * projection) noexcept;

            void setCell(const grid_node_leaf& leaf) noexcept;

            const grid_node_info& getInfo() const noexcept;
        };
    }
}