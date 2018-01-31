#pragma once

#include <cstddef>
#include <cmath>

#include <algorithm>

namespace math {
    struct vec4 {
        float x, y, z, w;

        constexpr vec4() noexcept:
            x(0.0F), 
            y(0.0F), 
            z(0.0F), 
            w(0.0F) {}

        constexpr vec4(float x, float y, float z, float w) noexcept: 
            x(x), 
            y(y), 
            z(z), 
            w(w) {}

        constexpr void data(float * ptr) const noexcept {
            ptr[0] = x;
            ptr[1] = y;
            ptr[2] = z;
            ptr[3] = w;
        }

        constexpr float min() const noexcept {
            return std::min(std::min(x, y), std::min(z, w));
        }

        constexpr float max() const noexcept {
            return std::max(std::max(x, y), std::max(z, w));
        }

        constexpr vec4 operator+ (vec4 other) const noexcept {
            return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        constexpr vec4 operator- (vec4 other) const noexcept {
            return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        constexpr vec4 operator* (vec4 other) const noexcept {
            return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
        }

        constexpr vec4 operator* (float scale) const noexcept {
            return vec4(x * scale, y * scale, z * scale, w * scale);
        }        

        constexpr vec4 operator/ (vec4 other) const noexcept {
            return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
        }

        constexpr vec4 operator/ (float scale) const noexcept {
            return vec4(x / scale, y / scale, z / scale, w / scale);
        }

        constexpr float dot(vec4 other) const noexcept {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        constexpr float length2() const noexcept {
            return x * x + y * y + z * z + w * w;
        }

        inline float length() const noexcept {
            return std::sqrt(length2());
        }

        inline vec4 normalize() const noexcept {
            auto invLen = 1.0F / length2();

            return vec4(x * invLen, y * invLen, z * invLen, w * invLen);
        }

        constexpr float& operator[](std::size_t idx) noexcept {
            switch (idx) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                case 3:
                    return w;
                default:
                    __builtin_trap();
            }
        }

        constexpr const float& operator[](std::size_t idx) const noexcept {
            switch (idx) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                case 3:
                    return w;
                default:
                    __builtin_trap();
            }
        }
    };

    constexpr vec4& operator+= (vec4& lhs, vec4 rhs) noexcept {
        return lhs = lhs + rhs;
    }

    constexpr vec4& operator-= (vec4& lhs, vec4 rhs) noexcept {
        return lhs = lhs - rhs;
    }

    constexpr vec4& operator*= (vec4& lhs, vec4 rhs) noexcept {
        return lhs = lhs * rhs;
    }

    constexpr vec4& operator*= (vec4& lhs, float rhs) noexcept {
        return lhs = lhs * rhs;
    }

    constexpr vec4& operator/= (vec4& lhs, vec4 rhs) noexcept {
        return lhs = lhs / rhs;
    }

    constexpr vec4& operator/= (vec4& lhs, float rhs) noexcept {
        return lhs = lhs / rhs;
    }
}