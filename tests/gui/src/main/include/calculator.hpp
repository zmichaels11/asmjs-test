#pragma once

#include "engine.hpp"

#include "gui_component.hpp"

namespace gui_test {
    class calculator : public gui_component {
        double _a, _b;
        double * _current;
        bool _set;
        int _prev;
        int _op;

    public:
        calculator() :
            _a(0.0),
            _b(0.0),
            _current(&_a),
            _set(false),
            _prev(0),
            _op(0) {}
            
        virtual void handle(engine::layers::gui_layer * pLayer) noexcept;
    };
}