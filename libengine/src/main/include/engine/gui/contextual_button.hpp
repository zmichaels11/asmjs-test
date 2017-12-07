#pragma once

#include <functional>
#include <string>

#include "engine/gui/button_info.hpp"
#include "engine/gui/contextual_widget.hpp"

namespace engine {
    namespace gui {                
        template<typename info_t>
        class contextual_button : public virtual contextual_widget {
            const info_t _info;
            std::function<void(const contextual_button*)> _onAction;
        
        public:
            contextual_button(const info_t& info) :
                _info(info),
                _onAction(nullptr) {}

            virtual ~contextual_button() {}

            virtual void build(void * ctx);

            virtual const info_t& getInfo() const;

            virtual void trigger() const;

            virtual void setOnAction(const std::function<void(const contextual_button*)>& callback);            
        };

        template<>
        void contextual_button<button_label_info>::build(void * ctx);

        template<>
        void contextual_button<button_text_info>::build(void * ctx);

        template<typename info_t>
        const info_t& contextual_button<info_t>::getInfo() const {
            return _info;
        }

        template<typename info_t>
        void contextual_button<info_t>::trigger() const {
            if (_onAction) {
                _onAction(this);
            }
        }

        template<typename info_t>
        void contextual_button<info_t>::setOnAction(const std::function<void(const contextual_button*)>& callback) {
            _onAction = callback;
        }
    }
}