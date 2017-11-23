#pragma once


#include <cstddef>

#include <fstream>
#include <memory>
#include <string>

namespace util {
    template<class T>
    inline T alignUp(T a, T b) {
        return (a + b - 1) / b * b;
    }

    inline std::unique_ptr<char[]> readAll(const std::string& file) {
        std::ifstream in(file, std::ios::binary | std::ios::ate);
        const auto len = in.tellg();
    
        auto out = std::make_unique<char[]> (std::size_t(len));
    
        in.seekg(0, std::ios::beg);
        in.read(out.get(), len);
    
        return out;
    }
}