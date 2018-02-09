#pragma once

#include "engine.hpp"

#include "gui_component.hpp"

namespace gui_test {
    class overview : public gui_component {
        bool _showMenu;
        bool _titleBar;
        bool _border;
        bool _resize;
        bool _movable;
        bool _noScrollbar;
        bool _scaleLeft;
        bool _minimizable;
        engine::layers::nuklear::panel_flags _windowFlags;
        
        engine::layers::nuklear::header_align _headerAlign;
        bool _showAppAbout;

        enum class menu_states {
            DEFAULT,
            MENU
        };

        struct menu_bar_t {
            unsigned int mprog;
            int mslider;
            bool mcheck;

            struct menu_item_t {
                unsigned int prog;
                int slider;
                bool check;

                menu_item_t() :
                    prog(40),
                    slider(10),
                    check(true) {}
            } menu;

            menu_bar_t() :
                mprog(60),
                mslider(10),
                mcheck(true)
                menu() {}
        } _menuBar;        

    public:
        overview() :
            _showMenu(false),
            _titleBar(false),
            _border(false),
            _resize(false),
            _movable(false),
            _noScrollbar(false),            
            _scaleLeft(false),
            _minimizable(false),
            _windowFlags(static_cast<engine::layers::nuklear::panel_flags> (0)),
            _headerAlign(engine::layers::nuklear::header_align::RIGHT),
            _showAppAbout(false),
            _menuBar() {}

        virtual void handle(engine::layers::gui_layer * pLayer) noexcept;
    };
}