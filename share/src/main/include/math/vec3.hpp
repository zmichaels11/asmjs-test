#pragma once

#include <cmath>
#include <cstddef>

namespace math {
    struct vec3 {
        float x, y, z;

        constexpr vec3() : x(0.0F), y(0.0F), z(0.0F) {}

        constexpr vec3(float x, float y, float z) : x(x), y(y), z(y) {}    

        constexpr vec3 operator+ (vec3 other) const {
            return vec3(x + other.x, y + other.y, z + other.z);
        }

        constexpr vec3 operator- (vec3 other) const {
            return vec3(x - other.x, y - other.y, z - other.z);
        }

        constexpr vec3 operator* (vec3 other) const {
            return vec3(x * other.x, y * other.y, z * other.z);
        }

        constexpr vec3 operator/ (vec3 other) const {
            return vec3(x / other.x, y / other.y, z / other.z);
        }

        constexpr float dot(vec3 other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        constexpr float length2() const {
            return x * x + y * y + z * z;
        }

        inline float length() const {
            return std::sqrt(length2());
        }
    
        inline vec3 normalize() const {
            auto invLen = 1.0F / length();

            return vec3(x * invLen, y * invLen, z * invLen);
        }

        constexpr float& operator[](std::size_t idx) {
            switch (idx) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
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
                default:
                    __builtin_trap();
            }
        }

        static constexpr vec3 cross(vec3 lhs, vec3 rhs) {
            return vec3(
                lhs.y * rhs.z - lhs.z * rhs.y,
                lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.x * rhs.y - lhs.y * rhs.x);
        }
    };
}