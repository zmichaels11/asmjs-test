#pragma once

#include <cstddef>

namespace graphics {
    namespace uniform {
        void setUniform1(int locaction, std::size_t count, const unsigned int * value);

        inline void setUniform1(int location, unsigned int value) {
            setUniform1(location, 1, &value);
        }

        void setUniform1(int location, std::size_t count, const int * value);

        inline void setUniform1(int location, int value) {
            setUniform1(location, 1 &value);
        }

        void setUniform1(int location, std::size_t count, const float * value);

        inline void setUniform1(int location, float value) {
            setUniform1(location, 1, &value);
        }

        void setUniform2(int location, std::size_t count, const unsigned int * value);

        inline void setUniform2(int location, unsigned int x, unsigned int y) {
            const unsigned int value[2] = {x, y};

            setUniform2(location, 1, value);
        }

        void setUniform2(int location, std::size_t count, const int * value);

        inline void setUniform2(int location, int x, int y) {
            const int value[2] = {x, y};

            setUniform2(location, 1, value);
        }

        void setUniform2(int location, std::size_t count, const float * value);

        inline void setUniform2(int location, float x, float y) {
            const float value[2] = {x, y};

            setUniform2(location, 1, value);
        }

        void setUniform3(int location, std::size_t count, const unsigned int * value);

        inline void setUniform3(int location, unsigned int x, unsigned int y, unsigned int z) {
            const unsigned int value[3] = {x, y, z};

            setUniform3(location, 1, value);
        }

        void setUniform3(int location, std::size_t count, const int * value);

        inline void setUniform3(int location, int x, int y, int z) {
            const int value[3] = {x, y, z};

            setUniform3(location, 1, value);
        }

        void setUniform3(int location, std::size_t count, const float * value);

        inline void setUniform3(int location, float x, float y, float z) {
            const float value[3] = {x, y, z};

            setUniform3(location, 1, value);
        }

        void setUniform4(int location, std::size_t count, const unsigned int * value);

        inline void setUniform4(int location, unsigned int x, unsigned int y, unsigned int z, unsigned int w) {
            const unsigned int value[4] = {x, y, z, w};

            setUniform4(location, 1, value);
        }

        void setUniform4(int location, std::size_t count, const int * value);

        inline void setUniform4(int location, int x, int y, int z, int w) {
            const int value[4] = {x, y, z, w};

            setUniform4(location, 1, value);
        }

        void setUniform4(int location, std::size_t count, const float * value);

        inline void setUniform4(int location, float x, float y, float z, float w) {
            const float value[4] = {x, y, z, w};

            setUniform4(location, 1, value);
        }

        void setUniformMatrix2(int location, std::size_t count, const float * value);

        void setUniformMatrix3(int location, std::size_t count, const float * value);

        void setUniformMatrix4(int location, std::size_t count, const float * value);    
    }
}