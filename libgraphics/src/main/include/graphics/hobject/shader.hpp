#pragma once

#include <string>
#include <utility>

#include "graphics/hinfo/shader_info.hpp"

namespace graphics {
    class program;

    class shader {
        graphics::shader_info _info;
        unsigned int _handle;
        bool _external;
        std::string _name;

        shader(const shader&) = delete;

        shader& operator=(const shader&) = delete;

        friend class program;

    public:
        inline shader(shader&& other) noexcept {
            _info = std::move(other._info);
            _name = std::move(other._name);
            _handle = other._handle;
            _external = other._external;

            other._handle = 0;
        }

        inline shader& operator=(shader&& other) {
            std::swap(_info, other._info);
            std::swap(_name, other._name);
            std::swap(_handle, other._handle);
            std::swap(_external, other._external);

            return *this;
        }

        shader() noexcept: 
            _info(),
            _handle(0),
            _external(false),
            _name("0") {}
        
        shader(const shader_info& info) noexcept;

        shader(unsigned int handle) noexcept:
            _info(),
            _handle(handle),
            _external(true),
            _name(std::to_string(handle)) {}        

        ~shader() noexcept;

        inline operator unsigned int() const noexcept {
            return _handle;
        }

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;

        const graphics::shader_info& getInfo() const noexcept;

        static shader makeVertex(const std::string& srcOrFile) noexcept;

        static shader makeFragment(const std::string& srcOrFile) noexcept;

        static shader makeGeometry(const std::string& srcOrFile) noexcept;

        static shader makeTessControl(const std::string& srcOrFile) noexcept;

        static shader makeTessEvaluation(const std::string& srcOrFile) noexcept;

        static shader makeCompute(const std::string& srcOrFile) noexcept;
    };
}