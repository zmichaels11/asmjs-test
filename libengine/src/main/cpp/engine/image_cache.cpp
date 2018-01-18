#include "engine/image_cache.hpp"

#include <map>
#include <string>

#include "graphics/image.hpp"

namespace engine {
    namespace {
        std::map<std::string, std::unique_ptr<graphics::image>> _images;
    }

    void image_cache::gc() noexcept {
        _images.clear();
    }

    const graphics::image * image_cache::getImage(const std::string& imgRef) noexcept {
        auto it = _images.find(imgRef);

        if (it == _images.end()) {
            auto result = _images.emplace(imgRef, graphics::image::read(imgRef));

            return result.first->second.get();
        } else {
            return it->second.get();
        }        
    }
}