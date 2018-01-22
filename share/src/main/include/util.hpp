#pragma once

#include <cstddef>

#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <streambuf>
#include <string>

class util {
    util() = delete;

public:
    inline static bool hasSuffix(const std::string& str, const std::string& suffix);

    template<class T>
    static T alignUp(T a, T b);

    inline static std::string stringReadAll(const std::string& file);

    inline static std::unique_ptr<char[]> readAll(const std::string& file);

    template<class T>
    inline static T bestFitPowerOf2(T value);

    template<class T>
    inline static T optimalMipmapCount(T value);

    template<class T>
    inline static T unorm(float value);

    template<class T>
    inline static float normalize(T value);
};

bool util::hasSuffix(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
            str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
} 

template<class T>
T util::alignUp(T a, T b) {
    return (a + b - 1) / b * b;
}

std::string util::stringReadAll(const std::string& file) {
    auto in = std::ifstream(file);
    auto out = std::string();

    in.seekg(0, std::ios::end);
    out.reserve(in.tellg());
    in.seekg(0, std::ios::beg);

    out.assign(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());

    return out;
}

std::unique_ptr<char[]> util::readAll(const std::string& file) {
    auto in = std::ifstream(file, std::ios::binary | std::ios::ate);
    auto len = in.tellg();
    auto out = std::make_unique<char[]> (std::size_t(len));

    in.seekg(0, std::ios::beg);
    in.read(out.get(), len);

    return out;
}

template<class T>
T util::optimalMipmapCount(T value) {
    auto result = static_cast<T> (0);  
    auto testValue = static_cast<T> (1);

    while (testValue < value) {        
        testValue <<= 1;
        result++;
    }

    return result;
}

template<class T>
T util::bestFitPowerOf2(T value) {        
    auto result = static_cast<T> (1);

    while (result < value) {
        result <<= 1;
    }

    return (result >> 1);
}

template<class T>
T util::unorm(float value) {
    return static_cast<T> (value * std::numeric_limits<T>::max());
}

template<class T>
float util::normalize(T value) {
    return static_cast<float> (value) / static_cast<float> (std::numeric_limits<T>::max());
}