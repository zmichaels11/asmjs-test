#pragma once

#include <map>
#include <memory>

#include "graphics/hinfo/pipeline_info.hpp"

#include "graphics/hobject/pipeline.hpp"
#include "graphics/hobject/program.hpp"

namespace graphics {
    class pipeline_cache {
        std::map<unsigned int, std::unique_ptr<program>> _programs;

    public:
        pipeline createPipeline(const pipeline_info& info) noexcept;        
    };
}