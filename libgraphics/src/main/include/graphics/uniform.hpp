#pragma once

#include <cstddef>

namespace graphics {
    namespace uniform {
        void setUniform1(int locaction, std::size_t count, const unsigned int * value);

        void setUniform1(int location, unsigned int value);

        void setUniform1(int location, std::size_t count, const int * value);

        void setUniform1(int location, int value);

        void setUniform1(int location, std::size_t count, const float * value);

        void setUniform1(int location, float value);

        void setUniform2(int location, std::size_t count, const unsigned int * value);

        void setUniform2(int location, unsigned int x, unsigned int y);

        void setUniform2(int location, std::size_t count, const int * value);

        void setUniform2(int location, int x, int y);

        void setUniform2(int location, std::size_t count, const float * value);

        void setUniform2(int location, float x, float y);

        void setUniform3(int location, std::size_t count, const unsigned int * value);

        void setUniform3(int location, unsigned int x, unsigned int y, unsigned int z);

        void setUniform3(int location, std::size_t count, const int * value);

        void setUniform3(int location, int x, int y, int z);

        void setUniform3(int location, std::size_t count, const float * value);

        void setUniform3(int location, float x, float y, float z);

        void setUniform4(int location, std::size_t count, const unsigned int * value);

        void setUniform4(int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w);

        void setUniform4(int location, std::size_t count, const int * value);

        void setUniform4(int location, int x, int y, int z, int w);

        void setUniform4(int location, std::size_t count, const float * value);

        void setUniform4(int location, float x, float y, float z, float w);

        void setUniformMatrix2(int location, std::size_t count, const float * value);

        void setUniformMatrix3(int location, std::size_t count, const float * value);

        void setUniformMatrix4(int location, std::size_t count, const float * value);    
    }
}