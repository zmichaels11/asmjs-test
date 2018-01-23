#pragma once

#include <cstddef>
#include <cmath>

namespace math {
    struct vec4 {
        float x, y, z, w;

        constexpr vec4() : x(0.0F), y(0.0F), z(0.0F), w(0.0F) {}

        constexpr vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        constexpr void data(float * ptr) const {
            ptr[0] = x;
            ptr[1] = y;
            ptr[2] = z;
            ptr[3] = w;
        }

        constexpr vec4 operator+ (vec4 other) const {
            return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        constexpr vec4 operator- (vec4 other) const {
            return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        constexpr vec4 operator* (vec4 other) const {
            return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
        }

        constexpr vec4 operator/ (vec4 other) const {
            return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
        }

        constexpr float dot(vec4 other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        constexpr float length2() const {
            return x * x + y * y + z * z + w * w;
        }

        inline float length() const {
            return std::sqrt(length2());
        }

        inline vec4 normalize() const {
            auto invLen = 1.0F / length2();

            return vec4(x * invLen, y * invLen, z * invLen, w * invLen);
        }

        constexpr float& operator[](std::size_t idx) {
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

        constexpr const float& operator[](std::size_t idx) const {
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
}