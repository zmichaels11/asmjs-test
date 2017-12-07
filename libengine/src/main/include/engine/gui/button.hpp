#pragma once

#include <functional>
#include <memory>

#include "engine/gui/button_info.hpp"
#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        template<typename info_t>
        class button : public virtual widget {
            std::function<void(const button<info_t>*)> _onAction;
            const info_t _info;

        public:
            button(const info_t& info) :
                _onAction(nullptr),
                _info(info) {}

            virtual ~button() {}

            virtual void build(void * ctx);

            virtual void setOnAction(const std::function<void(const button<info_t>*)>& callback);

            virtual void trigger() const;

            virtual const info_t& getInfo() const;                    
        };

        template<typename info_t>
        std::shared_ptr<button<info_t>> createButton(const info_t& info);
        
        template<typename info_t>
        std::shared_ptr<button<info_t>> createButton(const info_t& info) {
            return std::make_shared<button<info_t>> (info);
        }

        template<typename info_t>
        void button<info_t>::setOnAction(const std::function<void(const button<info_t>*)>& callback) {
            _onAction = callback;
        }

        template<typename info_t>
        void button<info_t>::trigger() const {
            if (_onAction) {
                _onAction(this);
            }
        }

        template<typename info_t>
        const info_t& button<info_t>::getInfo() const {
            return _info;
        }

        template<>
        void button<button_color_info>::build(void * ctx);

        template<>
        void button<button_label_info>::build(void * ctx);                

        template<>
        void button<button_symbol_info>::build(void * ctx);

        template<>
        void button<button_text_info>::build(void * ctx);
        
    }
}