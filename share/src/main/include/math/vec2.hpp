#pragma once

#include <cstddef>
#include <cmath>


namespace math {
    struct vec2 {
        float x, y;

        constexpr vec2() : x(0.0F), y(0.0F) {}

        constexpr vec2(float x, float y) : x(x), y(y) {}        

        constexpr vec2 operator+ (vec2 a) const {
            return vec2(x + a.x, y + a.y);
        }

        constexpr vec2 operator- (vec2 a) const {
            return vec2(x - a.x, y - a.y);
        }

        constexpr vec2 operator* (vec2 a) const {
            return vec2(x * a.x, y * a.y);
        }

        constexpr vec2 operator/ (vec2 a) const {
            return vec2(x / a.x, y / a.y);
        }

        constexpr float dot(vec2 a) const {
            return x * a.x + y * a.y;
        }
        
        constexpr float length2() const {
            return x * x + y * y;
        }

        inline float length() const {
            return std::sqrt(length2());
        }
        
        inline vec2 normalize() const {
            auto invLen = 1.0F / length();

            return vec2(x * invLen, y * invLen);
        }

        constexpr float& operator[] (std::size_t idx) {
            switch (idx) {
                case 0:
                    return x;
                case 1:
                    return y;
            }
        }

        constexpr const float& operator[] (std::size_t idx) const {
            switch (idx) {
                case 0:
                    return x;
                case 1:
                    return y;
            }
        }        
    };
}