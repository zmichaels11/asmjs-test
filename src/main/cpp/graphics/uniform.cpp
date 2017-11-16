#include "graphics/uniform.hpp"

#include <cstddef>

#include <GLES3/gl3.h>

namespace graphics {
    void uniform::setUniform1(int location, std::size_t count, const unsigned int * value) {
        glUniform1uiv(location, count, value);
    }

    void uniform::setUniform1(int location, std::size_t count, const int * value) {
        glUniform1iv(location, count, value);
    }

    void uniform::setUniform1(int location, std::size_t count, const float * value) {
        glUniform1fv(location, count, value);
    }

    void uniform::setUniform2(int location, std::size_t count, const unsigned int * value) {
        glUniform2uiv(location, count, value);
    }

    void uniform::setUniform2(int location, std::size_t count, const int * value) {
        glUniform2iv(location, count, value);
    }

    void uniform::setUniform2(int location, std::size_t count, const float * value) {
        glUniform2fv(location, count, value);
    }

    void uniform::setUniform3(int location, std::size_t count, const unsigned int * value) {
        glUniform3uiv(location, count, value);
    }

    void uniform::setUniform3(int location, std::size_t count, const int * value) {
        glUniform3iv(location, count, value);
    }

    void uniform::setUniform3(int location, std::size_t  count, const float * value) {
        glUniform3fv(location, count, value);
    }
    
    void uniform::setUniform4(int location, std::size_t count, const unsigned int * value) {
        glUniform4uiv(location, count, value);
    }

    void uniform::setUniform4(int location, std::size_t count, const int * value) {
        glUniform4iv(location, count, value);
    }

    void uniform::setUniform4(int location, std::size_t count, const float * value) {
        glUniform4fv(location, count, value);
    }

    void uniform::setUniformMatrix2(int location, std::size_t count, const float * value) {
        glUniformMatrix2fv(location, count, false, value);
    }

    void uniform::setUniformMatrix3(int location, std::size_t count, const float * value) {
        glUniformMatrix3fv(location, count, false, value);
    }

    void uniform::setUniformMatrix4(int location, std::size_t count, const float * value) {
        glUniformMatrix4fv(location, count, false, value);
    }
}