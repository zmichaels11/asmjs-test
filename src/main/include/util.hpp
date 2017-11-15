#pragma once

#include <memory>
#include <string>

namespace util {
    template<class T>
    inline T alignUp(T a, T b) {
        return (a + b - 1) / b * b;
    }

    std::unique_ptr<char[]> readAll(const std::string& file);
}