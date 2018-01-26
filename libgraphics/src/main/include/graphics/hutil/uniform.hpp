#pragma once

#include <cstddef>

namespace graphics {
    namespace uniform {

        void setUniform1(int locaction, std::size_t count, const unsigned int * value) noexcept;

        void setUniform1(int location, unsigned int value) noexcept;

        void setUniform1(int location, std::size_t count, const int * value) noexcept;

        void setUniform1(int location, int value) noexcept;

        void setUniform1(int location, std::size_t count, const float * value) noexcept;

        void setUniform1(int location, float value) noexcept;

        void setUniform2(int location, std::size_t count, const unsigned int * value) noexcept;

        void setUniform2(int location, unsigned int x, unsigned int y) noexcept;

        void setUniform2(int location, std::size_t count, const int * value) noexcept;

        void setUniform2(int location, int x, int y) noexcept;

        void setUniform2(int location, std::size_t count, const float * value) noexcept;

        void setUniform2(int location, float x, float y) noexcept;

        void setUniform3(int location, std::size_t count, const unsigned int * value) noexcept;

        void setUniform3(int location, unsigned int x, unsigned int y, unsigned int z) noexcept;

        void setUniform3(int location, std::size_t count, const int * value) noexcept;

        void setUniform3(int location, int x, int y, int z) noexcept;

        void setUniform3(int location, std::size_t count, const float * value) noexcept;

        void setUniform3(int location, float x, float y, float z) noexcept;

        void setUniform4(int location, std::size_t count, const unsigned int * value) noexcept;

        void setUniform4(int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) noexcept;

        void setUniform4(int location, std::size_t count, const int * value) noexcept;

        void setUniform4(int location, int x, int y, int z, int w) noexcept;

        void setUniform4(int location, std::size_t count, const float * value) noexcept;

        void setUniform4(int location, float x, float y, float z, float w) noexcept;

        void setUniformMatrix2(int location, std::size_t count, const float * value) noexcept;

        void setUniformMatrix3(int location, std::size_t count, const float * value) noexcept;

        void setUniformMatrix4(int location, std::size_t count, const float * value) noexcept;
    }
}