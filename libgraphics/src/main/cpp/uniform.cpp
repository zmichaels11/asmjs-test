#include "graphics/uniform.hpp"

#include <cstddef>

#include <GLES3/gl3.h>

namespace graphics {
    void uniform::setUniform1(int location, std::size_t count, const unsigned int * value) {
        glUniform1uiv(location, count, value);
    }

    void uniform::setUniform1(int location, unsigned int x) {
        glUniform1ui(location, x);
    }

    void uniform::setUniform1(int location, std::size_t count, const int * value) {
        glUniform1iv(location, count, value);
    }

    void uniform::setUniform1(int location, int x) {
        glUniform1i(location, x);
    }

    void uniform::setUniform1(int location, std::size_t count, const float * value) {
        glUniform1fv(location, count, value);
    }

    void uniform::setUniform1(int location, float x) {
        glUniform1f(location, x);
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

    void uniform::setUniform2(int location, unsigned int x, unsigned int y) {
        glUniform2ui(location, x, y);
    }

    void uniform::setUniform2(int location, int x, int y) {
        glUniform2i(location, x, y);
    }

    void uniform::setUniform2(int location, float x, float y) {
        glUniform2f(location, x, y);
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

    void uniform::setUniform3(int location, unsigned int x, unsigned int y, unsigned int z) {
        glUniform3ui(location, x, y, z);
    }

    void uniform::setUniform3(int location, int x, int y, int z) {
        glUniform3i(location, x, y, z);
    }

    void uniform::setUniform3(int location, float x, float y, float z) {
        glUniform3f(location, x, y, z);
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

    void uniform::setUniform4(int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) {
        glUniform4ui(location, x, y, z, w);
    }

    void uniform::setUniform4(int location, int x, int y, int z, int w) {
        glUniform4i(location, x, y, z, w);
    }

    void uniform::setUniform4(int location, float x, float y, float z, float w) {
        glUniform4f(location, x, y, z, w);
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