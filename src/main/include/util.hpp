#pragma once

namespace util {
    template<class T>
    inline T alignUp(T a, T b) {
        return (a + b - 1) / b * b;
    }
}