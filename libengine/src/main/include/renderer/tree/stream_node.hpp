#pragma once

#include <memory>

#include "renderer/color_transform.hpp"
#include "renderer/tree/image.hpp"
#include "renderer/tree/node.hpp"
#include "renderer/tree/stream_node_info.hpp"
#include "renderer/tree/stream_node_leaf.hpp"

namespace renderer {
    namespace tree {        
        class stream_node : public virtual node {
            struct resources_t {
                virtual ~resources_t() {}
            };

            std::unique_ptr<resources_t> _pResources;

            stream_node(const stream_node&) = delete;

            stream_node& operator=(const stream_node&) = delete;

        public:
            stream_node(stream_node&&) = default;

            stream_node& operator=(stream_node&&) = default;

            stream_node(const stream_node_info& info) noexcept;            

            virtual ~stream_node();

            virtual void setProjection(const float * projection) noexcept;

            void accept(const stream_node_leaf& leaf) noexcept;

            void setColorTransform(const renderer::color_transform& colorTransform) noexcept;

            const stream_node_info& getInfo() const noexcept;            

            inline stream_node& operator<< (const stream_node_leaf& leaf) noexcept;
        };

        stream_node& stream_node::operator<< (const stream_node_leaf& leaf) noexcept {
            accept(leaf);
            return *this;
        }
    }
}