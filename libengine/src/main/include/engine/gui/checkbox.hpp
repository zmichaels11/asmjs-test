#pragma once

#include <functional>
#include <string>

#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        class checkbox : public virtual widget {            
            const std::string _label;
            std::function<void(const checkbox*)> _onChange;
            bool _active;

        public:
            checkbox(const std::string& label, bool active) :
                _label(label),
                _active(active) {}

            virtual ~checkbox() {}

            virtual void build(void * ctx);

            virtual void setOnChange(const std::function<void(const checkbox*)>& callback);

            virtual void trigger() const;

            virtual bool isActive() const;

            virtual const std::string& getLabel() const;
        };
    }
}