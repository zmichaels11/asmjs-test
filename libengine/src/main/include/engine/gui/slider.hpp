#pragma once

#include <functional>

#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        template<typename value_t>
        class slider : public virtual widget {
            std::function<void(const slider<value_t>*)> _onChange;
            value_t _min, _max, _value, _step;

        public:
            slider(value_t min, value_t max, value_t step, value_t value) :
                _min(min),
                _max(max),
                _value(value),
                _step(step),
                _onChange(nullptr) {}

            virtual ~slider() {}

            virtual void build(void *);

            virtual value_t getMax() const;

            virtual value_t getMin() const;

            virtual value_t getStep() const;

            virtual value_t getValue() const;

            virtual void setOnChange(const std::function<void(const slider<value_t>*)>& callback);
        };

        template<typename value_t>
        value_t slider<value_t>::getMax() const {
            return _max;
        }

        template<typename value_t>
        value_t slider<value_t>::getMin() const {
            return _min;
        }

        template<typename value_t>
        value_t slider<value_t>::getValue() const {
            return _value;
        }

        template<typename value_t>
        value_t slider<value_t>::getStep() const {
            return _step;
        }

        template<typename value_t>
        void slider<value_t>::setOnChange(const std::function<void(const slider<value_t>*)>& callback) {
            _onChange = callback;
        }        

        template<>
        void slider<int>::build(void *);

        template<>
        void slider<float>::build(void *);
    }    
}