#pragma once

#include <memory>
#include <string>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/bounds.hpp"
#include "engine/layers/gui_layer_info.hpp"

#include "engine/layers/nuklear/edit_flags.hpp"
#include "engine/layers/nuklear/layout_format.hpp"
#include "engine/layers/nuklear/panel_flags.hpp"
#include "engine/layers/nuklear/plugin_filter.hpp"

namespace engine {
    namespace layers {
        class gui_layer : public base_layer {
            std::unique_ptr<base_resources> _pResources;

            gui_layer(const gui_layer&) = delete;

            gui_layer& operator=(const gui_layer&) = delete;

        public:
            gui_layer() {}

            gui_layer(const gui_layer_info& info) noexcept;

            gui_layer(gui_layer&&) = default;
            
            gui_layer& operator=(gui_layer&&) = default;
            
            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void invalidate() noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            void begin(
                const std::string& title, 
                const bounds<float, float>& bounds, 
                nuklear::panel_flags flags) noexcept;

            void end() noexcept;            

            void layoutRowDynamic(
                float height, 
                int cols) noexcept;

            void layoutRowStatic(
                float height, 
                int itemWidth, 
                int cols) noexcept;
            
            void buttonLabel(const std::string& title) noexcept;

            void buttonText(
                const char * title, 
                int len) noexcept;

            void edit_string(
                nuklear::edit_flags flags,
                char * buffer,
                int * len,
                int max,
                nuklear::plugin_filter filter) noexcept;
        };
    }
}