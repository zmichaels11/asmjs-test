#pragma once

#include <memory>
#include <string>

#include "graphics/image.hpp"

namespace graphics {    
    namespace image_io {
        std::unique_ptr<graphics::image> read(const std::string& path, unsigned int forcedChannels = 0);
    }
}