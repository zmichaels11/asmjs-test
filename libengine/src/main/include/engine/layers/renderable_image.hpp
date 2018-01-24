#pragma once

#include <memory>

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/renderable.hpp"
#include "engine/layers/renderable_image_info.hpp"

namespace engine {
    namespace layers {
        class renderable_image : public renderable {
            std::unique_ptr<base_resources> _pResources;

            renderable_image(const renderable_image&) = delete;

            renderable_image& operator=(const renderable_image&) = delete;

        public:
            renderable_image(renderable_image&&) = default;

            renderable_image& operator=(renderable_image&&) = default;

            renderable_image() : _pResources(nullptr) {}

            renderable_image(
                const context * pctx,
                const renderable_image_info& info) noexcept;

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void invalidate() noexcept;

            virtual void render() const noexcept;            

            virtual const void * getTexture() const noexcept;

            void setImage(const graphics::image * pImage) noexcept;

            const renderable_image_info& getInfo() const noexcept;
        };
    }
}