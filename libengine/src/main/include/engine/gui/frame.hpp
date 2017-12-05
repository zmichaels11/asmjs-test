#pragma once

#include <cstddef>

#include <memory>
#include <string>
#include <vector>

#include "engine/gui/container.hpp"
#include "engine/gui/frame_opts.hpp"

namespace engine {
    namespace gui {
        class frame : public virtual component {
            std::string _name;
            std::string _title;
            int _x, _y;
            unsigned int _w, _h;
            std::vector<std::shared_ptr<component>> _children;
            frame_opts _opts;

        public:
            frame() {}

            frame(frame_opts opts, int x, int y, unsigned int w, unsigned int h, const std::string& title, const std::string& name) :
                _opts(opts),
                _name(name),
                _title(title),
                _x(x), _y(y),
                _w(w), _h(h),
                _children() {}

            frame(frame_opts opts, int x, int y, unsigned int w, unsigned int h, const std::string& title) :
                _opts(opts),
                _name(title),
                _title(title),
                _x(x), _y(y),
                _w(w), _h(h),
                _children() {}

            virtual void build(void * ctx);

            inline void setChildren(const std::vector<std::shared_ptr<component>>& children);

            inline const std::string& getName() const;

            inline const std::string& getTitle() const;

            inline std::size_t getNumChildren() const;

            inline frame_opts getOptions() const;
        };

        void frame::setChildren(const std::vector<std::shared_ptr<component>>& children) {
            _children = children;
        }

        const std::string& frame::getName() const {
            return _name;
        }

        const std::string& frame::getTitle() const {
            return _title;
        }

        frame_opts frame::getOptions() const {
            return _opts;
        }

        std::size_t frame::getNumChildren() const {
            return _children.size();
        }
    }
}