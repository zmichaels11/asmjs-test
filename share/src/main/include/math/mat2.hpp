#pragma once

#include <cstddef>
#include <cmath>

#include "math/vec2.hpp"

namespace math {
    struct mat2 {
        vec2 a, b;

        constexpr mat2() : 

            a(), 
            b() {}

        constexpr mat2(vec2 a, vec2 b) : 

            a(a),
            b(b) {}

        constexpr mat2(
            float ax, float ay, 
            float bx, float by) :

            a(ax, ay),
            b(bx, by) {}

        constexpr void data(float * ptr) const {
            a.data(ptr);
            b.data(ptr + 2);
        }

        constexpr vec2& operator[] (std::size_t idx) {
            return (idx == 0) ? a : b;
        }

        constexpr const vec2& operator[] (std::size_t idx) const {
            return (idx == 0) ? a : b;
        }

        constexpr mat2 operator+ (mat2 other) const {
            return mat2(a + other.a, b + other.b);
        }

        constexpr mat2 operator- (mat2 other) const {
            return mat2(a - other.a, b - other.b);
        }

        constexpr mat2 operator* (float scale) const {
            return mat2(a * scale, b * scale);
        }

        constexpr mat2 operator/ (float scale) const {
            return mat2(a / scale, b / scale);
        }

        constexpr mat2 transpose() const {
            return mat2(
                a.x, b.x,
                a.y, b.y);
        }

        static constexpr mat2 scale(float sx, float sy) {
            return mat2(sx, 0.0, 0.0, sy);
        }

        static constexpr mat2 zero() {
            return scale(0.0, 0.0);
        }

        static constexpr mat2 identity() {
            return scale(1.0, 1.0);
        }

        static constexpr mat2 scale(vec2 sv) {
            return scale(sv.x, sv.y);
        }

        private:
            static constexpr mat2 _rotate(float sa, float ca) {
                return mat2(
                    ca, sa,
                    -sa, ca);
            }

        public:
        static mat2 rotate(float angle) {
            return _rotate(std::sin(angle), std::cos(angle));
        }
    };
}