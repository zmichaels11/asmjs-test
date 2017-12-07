#pragma once

#include <functional>
#include <string>

#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        template<class type_t>
        class property : public virtual widget {            
            const std::string _label;
            const float _incPerPixel;
            const type_t _min, _max, _step;
            type_t _value;            
            std::function<void(const property<type_t>*)> _onChange;

        public:
            property(const std::string& label, type_t min, type_t max, type_t step, type_t value, float incPerPixel = 1.0F) :
                _label(label),
                _incPerPixel(incPerPixel),
                _min(min),
                _max(max),
                _step(step),
                _value(value) {}

            virtual ~property() {}

            virtual void build(void * ctx);

            virtual const std::string& getLabel() const;

            virtual float getIncPerPixel() const;

            virtual void getRange(type_t& min, type_t& max, type_t& step) const;

            virtual type_t getValue() const;

            virtual void trigger() const;

            virtual void setOnChange(const std::function<void(const property<type_t> *)>& callback);
        };

        template<class type_t>
        const std::string& property<type_t>::getLabel() const {
            return _label;
        }

        template<class type_t>
        float property<type_t>::getIncPerPixel() const {
            return _incPerPixel;
        }

        template<class type_t>
        void property<type_t>::getRange(type_t& min, type_t& max, type_t& step) const {
            min = _min;
            max = _max;
            step = _step;
        }

        template<class type_t>
        type_t property<type_t>::getValue() const {
            return _value;
        }

        template<class type_t>
        void property<type_t>::trigger() const {
            if (_onChange) {
                _onChange(this);
            }
        }

        template<class type_t>
        void property<type_t>::setOnChange(const std::function<void(const property<type_t>*)>& callback) {
            _onChange = callback;
        }

        template<>
        void property<int>::build(void * ctx);

        template<>
        void property<float>::build(void * ctx);

        template<>
        void property<double>::build(void * ctx);
    }
}