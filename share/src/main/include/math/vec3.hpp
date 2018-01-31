#pragma once

#include <cmath>
#include <cstddef>

#include <algorithm>

namespace math {
    struct vec3 {
        float x, y, z;

        constexpr vec3() noexcept: 
            x(0.0F), 
            y(0.0F), 
            z(0.0F) {}

        constexpr vec3(float x, float y, float z) noexcept: 
            x(x), 
            y(y), 
            z(y) {}

        constexpr float min() const noexcept {
            return std::min(std::min(x, y), z);
        }

        constexpr float max() const noexcept {
            return std::max(std::max(x, y), z);
        }

        constexpr vec3 operator+ (vec3 other) const noexcept {
            return vec3(x + other.x, y + other.y, z + other.z);
        }

        constexpr vec3 operator- (vec3 other) const noexcept {
            return vec3(x - other.x, y - other.y, z - other.z);
        }

        constexpr vec3 operator* (vec3 other) const noexcept {
            return vec3(x * other.x, y * other.y, z * other.z);
        }

        constexpr vec3 operator* (float scale) const noexcept {
            return vec3(x * scale, y * scale, z * scale);
        }

        constexpr vec3 operator/ (vec3 other) const noexcept {
            return vec3(x / other.x, y / other.y, z / other.z);
        }

        constexpr vec3 operator/ (float scale) const noexcept {
            return vec3(x / scale, y / scale, z / scale);
        }

        constexpr float dot(vec3 other) const noexcept {
            return x * other.x + y * other.y + z * other.z;
        }

        constexpr float length2() const noexcept {
            return x * x + y * y + z * z;
        }

        inline float length() const noexcept {
            return std::sqrt(length2());
        }
    
        inline vec3 normalize() const noexcept {
            auto invLen = 1.0F / length();

            return vec3(x * invLen, y * invLen, z * invLen);
        }

        constexpr float& operator[](std::size_t idx) noexcept {
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

        constexpr const float& operator[](std::size_t idx) const noexcept {
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

        static constexpr vec3 cross(vec3 lhs, vec3 rhs) noexcept {
            return vec3(
                lhs.y * rhs.z - lhs.z * rhs.y,
                lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.x * rhs.y - lhs.y * rhs.x);
        }
    };

    constexpr vec3& operator+= (vec3& lhs, vec3 rhs) noexcept {
        return lhs = lhs + rhs;
    }

    constexpr vec3& operator-= (vec3& lhs, vec3 rhs) noexcept {
        return lhs = lhs - rhs;
    }

    constexpr vec3& operator*= (vec3& lhs, vec3 rhs) noexcept {
        return lhs = lhs * rhs;
    }

    constexpr vec3& operator*= (vec3& lhs, float rhs) noexcept {
        return lhs = lhs * rhs;
    }

    constexpr vec3& operator/= (vec3& lhs, vec3 rhs) noexcept {
        return lhs = lhs / rhs;
    }

    constexpr vec3& operator/= (vec3& lhs, float rhs) noexcept {
        return lhs = lhs / rhs;
    }
}