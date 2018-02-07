#if defined(GLES30)
#include "pch.h"
#include "graphics/hutil/uniform.hpp"

namespace graphics {
    void uniform::setUniform1(int location, std::size_t count, const unsigned int * value) noexcept {
        glUniform1uiv(location, count, value);
    }

    void uniform::setUniform1(int location, unsigned int x) noexcept {
        glUniform1ui(location, x);
    }

    void uniform::setUniform1(int location, std::size_t count, const int * value) noexcept {
        glUniform1iv(location, count, value);
    }

    void uniform::setUniform1(int location, int x) noexcept {
        glUniform1i(location, x);
    }

    void uniform::setUniform1(int location, std::size_t count, const float * value) noexcept {
        glUniform1fv(location, count, value);
    }

    void uniform::setUniform1(int location, float x) noexcept {
        glUniform1f(location, x);
    }

    void uniform::setUniform2(int location, std::size_t count, const unsigned int * value) noexcept {
        glUniform2uiv(location, count, value);
    }    

    void uniform::setUniform2(int location, std::size_t count, const int * value) noexcept {
        glUniform2iv(location, count, value);
    }

    void uniform::setUniform2(int location, std::size_t count, const float * value) noexcept {
        glUniform2fv(location, count, value);
    }

    void uniform::setUniform2(int location, unsigned int x, unsigned int y) noexcept {
        glUniform2ui(location, x, y);
    }

    void uniform::setUniform2(int location, int x, int y) noexcept {
        glUniform2i(location, x, y);
    }

    void uniform::setUniform2(int location, float x, float y) noexcept {
        glUniform2f(location, x, y);
    }

    void uniform::setUniform3(int location, std::size_t count, const unsigned int * value) noexcept {
        glUniform3uiv(location, count, value);
    }

    void uniform::setUniform3(int location, std::size_t count, const int * value) noexcept {
        glUniform3iv(location, count, value);
    }

    void uniform::setUniform3(int location, std::size_t  count, const float * value) noexcept {
        glUniform3fv(location, count, value);
    }

    void uniform::setUniform3(int location, unsigned int x, unsigned int y, unsigned int z) noexcept {
        glUniform3ui(location, x, y, z);
    }

    void uniform::setUniform3(int location, int x, int y, int z) noexcept {
        glUniform3i(location, x, y, z);
    }

    void uniform::setUniform3(int location, float x, float y, float z) noexcept {
        glUniform3f(location, x, y, z);
    }
    
    void uniform::setUniform4(int location, std::size_t count, const unsigned int * value) noexcept {
        glUniform4uiv(location, count, value);
    }

    void uniform::setUniform4(int location, std::size_t count, const int * value) noexcept {
        glUniform4iv(location, count, value);
    }

    void uniform::setUniform4(int location, std::size_t count, const float * value) noexcept {
        glUniform4fv(location, count, value);
    }

    void uniform::setUniform4(int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) noexcept {
        glUniform4ui(location, x, y, z, w);
    }

    void uniform::setUniform4(int location, int x, int y, int z, int w) noexcept {
        glUniform4i(location, x, y, z, w);
    }

    void uniform::setUniform4(int location, float x, float y, float z, float w) noexcept {
        glUniform4f(location, x, y, z, w);
    }

    void uniform::setUniformMatrix2(int location, std::size_t count, const float * value) noexcept {
        glUniformMatrix2fv(location, count, false, value);
    }

    void uniform::setUniformMatrix3(int location, std::size_t count, const float * value) noexcept {
        glUniformMatrix3fv(location, count, false, value);
    }

    void uniform::setUniformMatrix4(int location, std::size_t count, const float * value) noexcept {
        glUniformMatrix4fv(location, count, false, value);
    }
}
#endif
