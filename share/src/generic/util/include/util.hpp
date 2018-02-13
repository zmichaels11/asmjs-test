#pragma once

#include <cstddef>

#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <streambuf>
#include <string>
#include <vector>

class util {
    util() = delete;

public:
    inline static bool hasSuffix(const std::string& str, const std::string& suffix) noexcept;

    template<class T>
    static T alignUp(T a, T b) noexcept;

    inline static std::string stringReadAll(const std::string& file) noexcept;

    inline static std::vector<char> readAll(const std::string& file) noexcept;

    template<class T>
    inline static T bestFitPowerOf2(T value) noexcept;

    template<class T>
    inline static T optimalMipmapCount(T width, T height, T depth) noexcept;

    template<class T>
    inline static T unorm(float value) noexcept;

    template<class T>
    inline static float normalize(T value) noexcept;
};

bool util::hasSuffix(const std::string& str, const std::string& suffix) noexcept {
    return str.size() >= suffix.size() &&
            str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
} 

template<class T>
T util::alignUp(T a, T b) noexcept {
    return (a + b - 1) / b * b;
}

std::string util::stringReadAll(const std::string& file) noexcept {
    auto in = std::ifstream(file, std::ios::in | std::ios::ate);

    if (in.good()) {
      auto len = in.tellg();
      auto out = std::string();

      out.reserve(len);
      in.seekg(0, std::ios::beg);
      out.assign(std::istreambuf_iterator<char> (in), std::istreambuf_iterator<char> ());
      
      return out;
    } else {
      std::cerr << "[util] io_error: Could not read from file: \"" << file << "\"!" << std::endl;
      __builtin_trap();
    }
}

std::vector<char> util::readAll(const std::string& file) noexcept {
    auto in = std::ifstream(file, std::ios::in | std::ios::binary | std::ios::ate);

    if (in.good()) {
      auto len = in.tellg();
      auto out = std::vector<char>();

      out.reserve(len);
      out.resize(len);

      in.seekg(0, std::ios::beg);
      in.read(out.data(), len);

      return out;
    } else {
      std::cerr << "[util] io_error: Could not read from file: \"" << file << "\"!" << std::endl;
      __builtin_trap();
    }
}

template<class T>
T util::optimalMipmapCount(T width, T height, T depth) noexcept {
    auto value = std::max(std::max(width, height), depth);
    auto result = static_cast<T> (0);  
    auto testValue = static_cast<T> (1);

    while (testValue < value) {        
        testValue <<= 1;
        result++;
    }

    return result;
}

template<class T>
T util::bestFitPowerOf2(T value) noexcept {
    auto result = static_cast<T> (1);

    while (result < value) {
        result <<= 1;
    }

    return result;
}

template<class T>
T util::unorm(float value) noexcept {
    return static_cast<T> (value * std::numeric_limits<T>::max());
}

template<class T>
float util::normalize(T value) noexcept {
    return static_cast<float> (value) / static_cast<float> (std::numeric_limits<T>::max());
}
