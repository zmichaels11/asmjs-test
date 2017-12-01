#pragma once

#include <functional>
#include <string>

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class int_slider : public virtual component {
            std::string _label;
            std::function<void(int_slider*)> _onChange;
            int _min, _max;
            int _value;
            int _step;
            float _incPerPx;

        public:
            int_slider(const std::string& label, int min, int max, int step, int value, float incPerPx = 1.0F) :
                _label(label),
                _min(min),
                _max(max),
                _step(step),
                _value(value),
                _incPerPx(incPerPx) {}

            virtual ~int_slider() {}

            virtual void build();

            inline const std::string& getLabel() const;

            inline int getMax() const;

            inline int getMin() const;

            inline void getRange(int& min, int& max) const;

            inline int getStep() const;

            inline int getValue() const;

            inline float getIncPerPixel() const;

            inline void setOnChange(const std::function<void(const int_slider *)>& callback);
        };

        const std::string& int_slider::getLabel() const {
            return _label;
        }

        int int_slider::getMin() const {
            return _min;
        }

        int int_slider::getMax() const {
            return _max;
        }

        void int_slider::getRange(int& min, int& max) const {
            min = _min;
            max = _max;
        }

        int int_slider::getValue() const {
            return _value;
        }

        int int_slider::getStep() const {
            return _step;
        }

        float int_slider::getIncPerPixel() const {
            return _incPerPx;
        }

        void int_slider::setOnChange(const std::function<void(const int_slider *)>& callback) {
            _onChange = callback;
        }
    }
}