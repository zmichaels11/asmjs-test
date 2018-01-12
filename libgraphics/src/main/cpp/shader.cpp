#include "graphics/hobject/shader.hpp"

#include <cstdio>

#include "graphics/hinfo/shader_info.hpp"

#include "util.hpp"

namespace graphics {
    namespace {
        std::string _srcFromSrcOrFile(const std::string& srcOrFile) noexcept;
    }

    const shader_info& shader::getInfo() const noexcept {
        return _info;
    }

    shader shader::makeVertex(const std::string& srcOrFile) noexcept {
        auto src = _srcFromSrcOrFile(srcOrFile);

        return shader(graphics::shader_info{graphics::shader_type::VERTEX, src});
    }

    shader shader::makeFragment(const std::string& srcOrFile) noexcept {
        auto src = _srcFromSrcOrFile(srcOrFile);

        return shader(graphics::shader_info{graphics::shader_type::FRAGMENT, src});
    }

    shader shader::makeGeometry(const std::string& srcOrFile) noexcept {
        auto src = _srcFromSrcOrFile(srcOrFile);

        return shader(graphics::shader_info{graphics::shader_type::GEOMETRY, src});
    }

    shader shader::makeTessControl(const std::string& srcOrFile) noexcept {
        auto src = _srcFromSrcOrFile(srcOrFile);

        return shader(graphics::shader_info{graphics::shader_type::TESS_CONTROL, src});
    }

    shader shader::makeTessEvaluation(const std::string& srcOrFile) noexcept {
        auto src = _srcFromSrcOrFile(srcOrFile);

        return shader(graphics::shader_info{graphics::shader_type::TESS_EVALUATION, src});
    }

    shader shader::makeCompute(const std::string& srcOrFile) noexcept {
        auto src = _srcFromSrcOrFile(srcOrFile);

        return shader(graphics::shader_info{graphics::shader_type::COMPUTE, src});
    }

    namespace {
        std::string _srcFromSrcOrFile(const std::string& srcOrFile) noexcept {
            static const std::string PREFIX("#version");

            if (!srcOrFile.compare(0, PREFIX.size(), PREFIX)) {
                return srcOrFile;
            } else {
                return util::stringReadAll(srcOrFile);
            }
        }
    }
}