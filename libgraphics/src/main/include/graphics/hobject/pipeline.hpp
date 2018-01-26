#pragma once

#include "graphics/hinfo/pipeline_info.hpp"

#include "graphics/hobject/program.hpp"

namespace graphics {    
    class pipeline {
        pipeline_info _info;
        const program  * _pProgram;

    public:
        pipeline(
            const pipeline_info& info,
            const program * pProgram) : 

            _info(info),
            _pProgram(pProgram){}

        inline const pipeline_info& getInfo() const noexcept {
            return _info;
        }

        inline const program * getProgram() const noexcept {
            return _pProgram;
        }
    };
}