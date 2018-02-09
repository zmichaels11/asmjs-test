#pragma once

#include <memory>
#include <string>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/bounds.hpp"
#include "engine/layers/color.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/gui_layer_info.hpp"
#include "engine/layers/point.hpp"

#include "engine/layers/nuklear/collapse_states.hpp"
#include "engine/layers/nuklear/color_format.hpp"
#include "engine/layers/nuklear/edit_flags.hpp"
#include "engine/layers/nuklear/edit_types.hpp"
#include "engine/layers/nuklear/header_align.hpp"
#include "engine/layers/nuklear/layout_format.hpp"
#include "engine/layers/nuklear/modify.hpp"
#include "engine/layers/nuklear/panel_flags.hpp"
#include "engine/layers/nuklear/plugin_filter.hpp"
#include "engine/layers/nuklear/popup_type.hpp"
#include "engine/layers/nuklear/text_alignment.hpp"
#include "engine/layers/nuklear/tree_type.hpp"

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

            // WINDOW

            bool begin(
                const std::string& title, 
                const bounds<float, float>& bounds, 
                nuklear::panel_flags flags) noexcept;

            void end() noexcept;

            // LAYOUT

            void layoutSetMinRowHeight(float height) noexcept;

            void layoutResetMinRowHeight() noexcept;

            void layoutWidgetBounds() noexcept;

            void layoutRatioFromPixel(float pixelWidth) noexcept;

            void layoutRowDynamic(
                float height, 
                int cols) noexcept;

            void layoutRowStatic(
                float height, 
                int itemWidth, 
                int cols) noexcept;

            template<nuklear::layout_format fmt>
            void layoutRowBegin(
                float rowHeight, 
                int cols) noexcept;

            void layoutRowPush(float ratioOrWidth) noexcept;

            void layoutRowEnd() noexcept;

            template<nuklear::layout_format fmt>
            void layoutRow( 
                float height, 
                int cols, 
                const float * pRatio) noexcept;

            void layoutRowTemplateBegin(float rowHeight) noexcept;

            void layoutRowTemplatePushDynamic() noexcept;

            void layoutRowTemplatePushVariable(float minWidth) noexcept;

            void layoutRowTemplatePushStatic(float width) noexcept;

            void layoutRowTemplateEnd() noexcept;

            template<nuklear::layout_format fmt>
            void layoutSpaceBegin(
                float height,
                int widgetCount) noexcept;

            void layoutSpacePush(const bounds<float, float>& rect) noexcept;

            void layoutSpaceEnd() noexcept;

            // GROUP

            bool groupBegin(
                const std::string& title,
                nuklear::panel_flags flags) noexcept;

            void groupEnd() noexcept;

            // TREE
            template<nuklear::tree_type type>
            void treePush( 
                const std::string& title, 
                nuklear::collapse_states state) noexcept;

            void treePop() noexcept;

            // WIDGET
            bounds<float, float> widgetBounds() noexcept;

            point<float> widgetPosition() noexcept;

            // TEXT

            void text(
                const char * txt, int len,
                nuklear::text_alignment alignment) noexcept;

            void text(
                const char * txt, int len,
                nuklear::text_alignment alignment,
                const color& c) noexcept;

            void textWrap(const char * txt, int len) noexcept;

            void textWrap(
                const char * txt, int len,
                const color& c) noexcept;

            void label(
                const std::string& label, 
                nuklear::text_alignment alignment) noexcept;

            void label(
                const std::string& label,
                nuklear::text_alignment alignment,
                const color& c) noexcept;

            void labelWrap(const std::string& label) noexcept;

            void labelWrap(
                const std::string& label,
                const color& c) noexcept;

            // BUTTON

            bool buttonText(const char * txt, int len) noexcept;

            bool buttonLabel(const std::string& txt) noexcept;

            // CHECKBOX

            bool checkLabel(const std::string& txt, bool active) noexcept;

            bool checkText(const char * txt, int len, bool active) noexcept;

            bool checkboxLabel(const std::string& label, bool * pActive) noexcept;

            bool checkboxText(const char * txt, int len, bool * pActive) noexcept;

            // RADIO BUTTON

            bool radioLabel(const std::string& label, bool * pActive) noexcept;

            bool radioText(const char * txt, int len, bool * pActive) noexcept;

            bool optionLabel(const std::string& label, bool enabled) noexcept;

            bool optionText(const char * txt, int len, bool enabled) noexcept;

            // SELECTABLE

            bool selectableLabel(
                const std::string& label, 
                nuklear::text_alignment align, 
                bool * pSelect) noexcept;

            bool selectLabel(
                const std::string& label, 
                nuklear::text_alignment align, 
                bool select) noexcept;

            bool selectableText(
                const char * txt, int len,
                nuklear::text_alignment align,
                bool * pSelect) noexcept;

            bool selectText(
                const char * txt, int len,
                nuklear::text_alignment align,
                bool select) noexcept;

            // SLIDER

            template<class prop_T>
            void slider(
                const std::string& label,
                prop_T min, prop_T * pValue, prop_T max, prop_T step) noexcept;

            // PROGRESS BAR
            template<nuklear::modify mod = nuklear::modify::FIXED>
            void progress(
                unsigned int * pCurrent, 
                unsigned int max) noexcept;

            // COLOR PICKER
            template<nuklear::color_format fmt = nuklear::color_format::RGB>
            color colorPicker(const color& c) noexcept;

            // PROPERTIES
            template<class prop_T>
            void property(
                const std::string& label, 
                prop_T min, prop_T * pValue, prop_T max, prop_T step, 
                float incPerPixel = 1.0F) noexcept;

            // TEXT EDIT

            template<nuklear::plugin_filter filter = nuklear::plugin_filter::DEFAULT>
            void editString(
                nuklear::edit_flags flags,
                char * buffer,
                int * len,
                int max) noexcept;

            // POPUP

            template<nuklear::popup_type type = nuklear::popup_type::STATIC>
            bool popupBegin(
                const std::string& title, 
                nuklear::panel_flags flags, 
                const bounds<float, float>& bounds) noexcept;

            void popupEnd() noexcept;

            void popupClose() noexcept;

            // COMBOBOX

            int combo(
                const char ** pItems, int count,
                int selected,
                int itemHeight,
                const point<float>& size) noexcept;

            bool comboBeginText(
                const char * txt, int len,
                const point<float>& size) noexcept;

            bool comboBeginLabel(
                const std::string& txt,
                const point<float>& size) noexcept;

            bool comboBeginColor(
                const color& c,
                const point<float>& size) noexcept;

            void comboClose() noexcept;

            void comboEnd() noexcept;

            // CONTEXTUAL

            bool contextualBegin(
                const point<float>& pos,
                const bounds<float, float>& triggerBounds) noexcept;

            bool contextualItemText(
                const char * txt, int len,
                nuklear::text_alignment align) noexcept;

            bool contextualItemLabel(
                const std::string& txt,
                nuklear::text_alignment align) noexcept;

            void contextualClose() noexcept;

            void contextualEnd() noexcept;

            // TOOLTIP

            void tooltip(const std::string& tip) noexcept;

            bool tooltipBegin(float width) noexcept;

            void tooltipEnd() noexcept;

            // MENU            

            void menubarBegin() noexcept;

            void menubarEnd() noexcept;

            bool menuBeginText(
                const char * txt, int len,
                nuklear::text_alignment align,
                const point<float>& size) noexcept;

            bool menuBeginLabel(
                const std::string& label, 
                nuklear::text_alignment align, 
                const point<float>& size) noexcept;

            bool menuItemLabel(
                const std::string& label,
                nuklear::text_alignment align) noexcept;

            bool menuItemText(
                const char * txt, int len,
                nuklear::text_alignment align) noexcept;

            void menuClose() noexcept;

            void menuEnd() noexcept;

            // STYLE

            void styleDefault() noexcept;

            void styleSetFont(int fontID) noexcept;

            void stylePushFont(int fontID) noexcept;

            void stylePushFloat(float * pF, float f) noexcept;

            void stylePushVec2(point<float>* pV, const point<float>& v) noexcept;

            void stylePopVec2() noexcept;

            void stylePopFloat() noexcept;
        };

        template<>
        void gui_layer::layoutRowBegin<nuklear::layout_format::STATIC>(
            float rowHeight, 
            int cols) noexcept;

        template<>
        void gui_layer::layoutRowBegin<nuklear::layout_format::DYNAMIC>(
            float rowHeight, 
            int cols) noexcept;

        template<>
        void gui_layer::layoutRow<nuklear::layout_format::DYNAMIC>( 
            float height, 
            int cols, 
            const float * pRatio) noexcept;

        template<>
        void gui_layer::layoutRow<nuklear::layout_format::STATIC>( 
            float height, 
            int cols, 
            const float * pRatio) noexcept;


        template<>
        void gui_layer::slider<float>(
            const std::string& label,
            float min, 
            float * pValue, 
            float max, 
            float step) noexcept;

        template<>
        void gui_layer::slider<int>(
            const std::string& label,
            int min, 
            int * pValue, 
            int max, 
            int step) noexcept;        

        template<>
        void gui_layer::property<int>(
            const std::string& label, 
            int min, int * pValue, int max, int step, 
            float incPerPixel) noexcept;

        template<>
        void gui_layer::property<float>(
            const std::string& label,
            float min, float * pValue, float max, float step,
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
