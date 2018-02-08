#pragma once

#include "gui_component.hpp"

namespace gui_test {    
    class demo : public gui_component {
        enum class op_type {
            EASY,
            HARD
        };

        op_type _op;
        int _property;

    public:
        demo() :
            _op(op_type::EASY),
            _property(20) {}
            
        virtual void handle(engine::layers::gui_layer * pLayer) noexcept;
    };
}