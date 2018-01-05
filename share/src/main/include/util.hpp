#pragma once


#include <cstddef>

#include <fstream>
#include <limits>
#include <memory>
#include <streambuf>
#include <string>

namespace util {
    inline bool hasSuffix(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() &&
                str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    } 

    template<class T>
    inline T alignUp(T a, T b) {
        return (a + b - 1) / b * b;
    }

    inline std::string stringReadAll(const std::string& file) {
        auto in = std::ifstream(file);
        auto out = std::string();

        in.seekg(0, std::ios::end);
        out.reserve(in.tellg());
        in.seekg(0, std::ios::beg);

        out.assign(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());

        return out;
    }

    inline std::unique_ptr<char[]> readAll(const std::string& file) {
        auto in = std::ifstream(file, std::ios::binary | std::ios::ate);
        auto len = in.tellg();
        auto out = std::make_unique<char[]> (std::size_t(len));
    
        in.seekg(0, std::ios::beg);
        in.read(out.get(), len);
    
        return out;
    }

    template<class T>
    inline T bestFitPowerOf2(T value) {        
        auto result = static_cast<T> (0);

        while (result < value) {
            result <<= 1;
        }

        return (result >> 1);
    }

    template<class T>
    inline T unorm(float value) {
        return static_cast<T> (value * std::numeric_limits<T>::max());
    }

    template<class T>
    inline float normalize(T value) {
        return static_cast<float> (value) / static_cast<float> (std::numeric_limits<T>::max());
    }
}