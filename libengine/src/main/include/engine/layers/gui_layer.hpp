#pragma once

#include <memory>
#include <string>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/bounds.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/gui_layer_info.hpp"

#include "engine/layers/nuklear/edit_flags.hpp"
#include "engine/layers/nuklear/edit_types.hpp"
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
            gui_layer(const context& ctx, const gui_layer_info& info) noexcept;

            gui_layer(gui_layer&&) = default;
            
            gui_layer& operator=(gui_layer&&) = default;
            
            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() noexcept;

            virtual void invalidate() noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            bool begin(
                const std::string& title, 
                const bounds<float, float>& bounds, 
                nuklear::panel_flags flags) noexcept;

            void end() noexcept;  

            bool optionLabel(const std::string& label, bool enabled) noexcept;

            template<class prop_T>
            void property(
                const std::string& label, 
                prop_T min, prop_T * pValue, prop_T max, prop_T step, 
                float incPerPixel = 1.0F) noexcept; 

            void layoutRowDynamic(
                float height, 
                int cols) noexcept;

            void layoutRowStatic(
                float height, 
                int itemWidth, 
                int cols) noexcept;
            
            bool buttonLabel(const std::string& label) noexcept;

            bool buttonText(
                const char * label, 
                int len) noexcept;

            template<nuklear::plugin_filter filter = nuklear::plugin_filter::DEFAULT>
            void editString(
                nuklear::edit_flags flags,
                char * buffer,
                int * len,
                int max) noexcept;
        };

        template<>
        void gui_layer::property<int>(
            const std::string& label, 
            int min, int * pValue, int max, int step, 
            float incPerPixel) noexcept;

        template<>
        void gui_layer::editString<nuklear::plugin_filter::ASCII>(
            nuklear::edit_flags flags,
            char * buffer,
            int * len,
            int max) noexcept;

        template<>
        void gui_layer::editString<nuklear::plugin_filter::BINARY>(
            nuklear::edit_flags flags,
            char * buffer,
            int * len,
            int max) noexcept;

        template<>
        void gui_layer::editString<nuklear::plugin_filter::DECIMAL>(
            nuklear::edit_flags flags,
            char * buffer,
            int * len,
            int max) noexcept;

        template<>
        void gui_layer::editString<nuklear::plugin_filter::DEFAULT>(
            nuklear::edit_flags flags,
            char * buffer,
            int * len,
            int max) noexcept;

        template<>
        void gui_layer::editString<nuklear::plugin_filter::FLOAT>(
            nuklear::edit_flags flags,
            char * buffer,
            int * len,
            int max) noexcept;

        template<>
        void gui_layer::editString<nuklear::plugin_filter::HEX>(
            nuklear::edit_flags flags,
            char * buffer,
            int * len,
            int max) noexcept;

        template<>
        void gui_layer::editString<nuklear::plugin_filter::OCT>(
            nuklear::edit_flags flags,
            char * buffer,
            int * len,
            int max) noexcept;
    }
}