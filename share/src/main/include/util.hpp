#pragma once


#include <cstddef>

#include <fstream>
#include <memory>
#include <streambuf>
#include <string>

namespace util {
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
}