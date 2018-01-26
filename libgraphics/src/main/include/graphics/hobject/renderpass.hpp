#pragma once

#include "graphics/hinfo/renderpass_info.hpp"

namespace graphics {
    class renderpass {
        renderpass_info _info;

    public:
        renderpass(const renderpass_info& info) :
            _info(info) {}

        inline const renderpass_info& getInfo() const noexcept {
            return _info;
        }
    };
}