#pragma once

#include <cstddef>

#include <string>

#include "engine/gui/component.hpp"
#include "engine/gui/frame_opts.hpp"

namespace engine {
    namespace gui {
        class frame : public component {
            std::string _name;
            std::string _title;
            int _x, _y;
            unsigned int _w, _h;
            component * _pChildren;
            std::size_t _numChildren;
            frame_opts _opts;

        public:
            frame() {}

            frame(frame_opts opts, int x, int y, unsigned int w, unsigned int h, const std::string& title, const std::string& name) :
                _opts(opts),
                _name(name),
                _title(title),
                _x(x), _y(y),
                _w(w), _h(h),
                _pChildren(nullptr),
                _numChildren(0) {}

            frame(frame_opts opts, int x, int y, unsigned int w, unsigned int h, const std::string& title) :
                _opts(opts),
                _name(title),
                _title(title),
                _x(x), _y(y),
                _w(w), _h(h),
                _pChildren(nullptr),
                _numChildren(0) {}

            virtual void build();

            void setChildren(component * pChildren, std::size_t count);
        };
    }
}