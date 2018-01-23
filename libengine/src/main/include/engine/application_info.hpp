#pragma once

#include <string>

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

        bool vsync;
    };
}