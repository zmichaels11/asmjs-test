#pragma once

#include <cstddef>
#include <cmath>

#include "math/vec2.hpp"

namespace math {
    struct mat2 {
        vec2 a, b;

        constexpr mat2() noexcept: 

            a(), 
            b() {}

        constexpr mat2(vec2 a, vec2 b) noexcept : 

            a(a),
            b(b) {}

        constexpr mat2(
            float ax, float ay, 
            float bx, float by) noexcept :

            a(ax, ay),
            b(bx, by) {}

        constexpr void data(float * ptr) const noexcept {
            a.data(ptr);
            b.data(ptr + 2);
        }

        constexpr vec2& operator[] (std::size_t idx) noexcept {
            return (idx == 0) ? a : b;
        }

        constexpr const vec2& operator[] (std::size_t idx) const noexcept {
            return (idx == 0) ? a : b;
        }

        constexpr mat2 operator+ (mat2 other) const noexcept {
            return mat2(a + other.a, b + other.b);
        }

        constexpr mat2 operator- (mat2 other) const noexcept {
            return mat2(a - other.a, b - other.b);
        }

        constexpr mat2 operator* (float scale) const noexcept {
            return mat2(a * scale, b * scale);
        }

        constexpr mat2 operator/ (float scale) const noexcept {
            return mat2(a / scale, b / scale);
        }

        constexpr mat2 transpose() const noexcept {
            return mat2(
                a.x, b.x,
                a.y, b.y);
        }

        static constexpr mat2 scale(float sx, float sy) noexcept {
            return mat2(sx, 0.0, 0.0, sy);
        }

        static constexpr mat2 zero() noexcept {
            return scale(0.0, 0.0);
        }

        static constexpr mat2 identity() noexcept {
            return scale(1.0, 1.0);
        }

        static constexpr mat2 scale(vec2 sv) noexcept {
            return scale(sv.x, sv.y);
        }

        private:
            static constexpr mat2 _rotate(float sa, float ca) noexcept {
                return mat2(
                    ca, sa,
                    -sa, ca);
            }

        public:
        static mat2 rotate(float angle) noexcept {
            return _rotate(std::sin(angle), std::cos(angle));
        }
    };

    constexpr mat2& operator+= (mat2& lhs, mat2 rhs) noexcept {
        return lhs = lhs + rhs;
    }

    constexpr mat2& operator-= (mat2& lhs, mat2 rhs) noexcept {
        return lhs = lhs - rhs;
    }

    constexpr mat2& operator*= (mat2& lhs, float rhs) noexcept {
        return lhs = lhs * rhs;
    }

    constexpr mat2& operator/= (mat2& lhs, float rhs) noexcept {
        return lhs = lhs / rhs;
    }
}