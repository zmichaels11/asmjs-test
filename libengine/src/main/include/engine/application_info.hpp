#pragma once

#include <string>

#include "engine/application_hint.hpp"

namespace engine {
    struct application_info {
        std::string name;
        struct window_dimensions_t {
            unsigned int width;
            unsigned int height;
        } window;

        struct api_t {
            unsigned int major;
            unsigned int minor;
        } apiVersion;

        application_hint hints;
    };
}