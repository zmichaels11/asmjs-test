#pragma once

#include <cstddef>

namespace graphics {
    namespace uniform {
        void setUniform1(int locaction, std::size_t count, const unsigned int * value);

        void setUniform1(int location, std::size_t count, const int * value);

        void setUniform1(int location, std::size_t count, const float * value);

        void setUniform2(int location, std::size_t count, const unsigned int * value);

        void setUniform2(int location, std::size_t count, const int * value);

        void setUniform2(int location, std::size_t count, const float * value);

        void setUniform3(int location, std::size_t count, const unsigned int * value);

        void setUniform3(int location, std::size_t count, const int * value);

        void setUniform3(int location, std::size_t count, const float * value);

        void setUniform4(int location, std::size_t count, const unsigned int * value);

        void setUniform4(int location, std::size_t count, const int * value);

        void setUniform4(int location, std::size_t count, const float * value);

        void setUniformMatrix2(int location, std::size_t count, const float * value);

        void setUniformMatrix3(int location, std::size_t count, const float * value);

        void setUniformMatrix4(int location, std::size_t count, const float * value);    
    }
}