#pragma once

#include <cstddef>
#include <cmath>

#include <algorithm>


namespace math {
    struct vec2 {
        float x, y;

        constexpr vec2() noexcept: 
            x(0.0F), 
            y(0.0F) {}

        constexpr vec2(float x, float y) noexcept: 
            x(x), 
            y(y) {}

        constexpr void data(float * ptr) const noexcept {
            ptr[0] = x;
            ptr[1] = y;
        }

        constexpr float min() const noexcept {
            return std::min(x, y);
        }

        constexpr float max() const noexcept {
            return std::max(x, y);
        }        

        constexpr vec2 operator+ (vec2 a) const noexcept {
            return vec2(x + a.x, y + a.y);
        }

        constexpr vec2 operator- (vec2 a) const noexcept {
            return vec2(x - a.x, y - a.y);
        }

        constexpr vec2 operator* (vec2 a) const noexcept {
            return vec2(x * a.x, y * a.y);
        }

        constexpr vec2 operator* (float s) const noexcept {
            return vec2(x * s, y * s);
        }

        constexpr vec2 operator/ (vec2 a) const noexcept {
            return vec2(x / a.x, y / a.y);
        }

        constexpr vec2 operator/ (float s) const noexcept {
            return vec2(x / s, y / s);
        }

        constexpr float dot(vec2 a) const noexcept {
            return x * a.x + y * a.y;
        }
        
        constexpr float length2() const noexcept {
            return x * x + y * y;
        }

        inline float length() const noexcept {
            return std::sqrt(length2());
        }
        
        inline vec2 normalize() const noexcept {
            auto invLen = 1.0F / length();

            return vec2(x * invLen, y * invLen);
        }

        constexpr float& operator[] (std::size_t idx) noexcept {
            return (idx == 0) ? x : y;
        }

        constexpr const float& operator[] (std::size_t idx) const noexcept {
            return (idx == 0) ? x : y;
        }        
    };

    constexpr vec2& operator+= (vec2& lhs, vec2 rhs) noexcept {
        return lhs = lhs + rhs;
    }

    constexpr vec2& operator-= (vec2& lhs, vec2 rhs) noexcept {
        return lhs = lhs - rhs;
    }

    constexpr vec2& operator*= (vec2& lhs, vec2 rhs) noexcept {
        return lhs = lhs * rhs;
    }

    constexpr vec2& operator*= (vec2& lhs, float rhs) noexcept {
        return lhs = lhs * rhs;
    }

    constexpr vec2& operator/= (vec2& lhs, vec2 rhs) noexcept {
        return lhs = lhs / rhs;
    }

    constexpr vec2& operator/= (vec2& lhs, float rhs) noexcept {
        return lhs = lhs / rhs;
    }
}