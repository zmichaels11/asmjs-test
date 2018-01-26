#include "graphics/hobject/pipeline_cache.hpp"

#include <memory>
#include <utility>

#include "graphics/hobject/program.hpp"

namespace graphics {
    pipeline pipeline_cache::createPipeline(const pipeline_info& info) noexcept {
        auto it = _programs.find(info.programState.uuid);

        if (it != _programs.end()) {
            return pipeline(info, it->second.get());
        }

        auto ptr = std::make_unique<program>(info.programState);
        auto out = ptr.get();

        _programs[info.programState.uuid] = std::move(ptr);    
    }
}