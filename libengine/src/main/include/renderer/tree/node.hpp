#pragma once

#include "renderer/color_transform.hpp"

namespace renderer {
    namespace tree {
        class node {
        public:
            virtual ~node() noexcept {};

            virtual void render(void * pRenderTarget) noexcept;

            virtual void setColorTransform(const renderer::color_transform& colorTransform) noexcept;

            virtual bool supportsColorTransform() const noexcept;

            virtual bool isVisible() const noexcept;

            virtual void setVisible(bool isVisible) noexcept;
        };
    }    
}