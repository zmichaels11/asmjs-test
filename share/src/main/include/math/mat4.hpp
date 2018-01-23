#pragma once

#include <cstddef>
#include <cmath>

#include "math/vec3.hpp"
#include "math/vec4.hpp"

namespace math {
    struct mat4 {
        vec4 a, b, c, d;

        constexpr mat4() : a(), b(), c(), d() {}

        constexpr mat4(vec4 a, vec4 b, vec4 c, vec4 d) : 

        a(a), 
        b(b), 
        c(c), 
        d(d) {}

        constexpr mat4(
            float ax, float ay, float az, float aw,
            float bx, float by, float bz, float bw,
            float cx, float cy, float cz, float cw,
            float dx, float dy, float dz, float dw) :
            
            a(ax, ay, az, aw),
            b(bx, by, bz, bw),
            c(cx, cy, cz, cw),
            d(dx, dy, dz, dw) {}

        constexpr void data(float * ptr) const {
            a.data(ptr);
            b.data(ptr+4);
            c.data(ptr+8);
            d.data(ptr+12);
        }

        constexpr vec4& operator[] (std::size_t idx) {
            switch (idx) {
                case 0:
                    return a;
                case 1:
                    return b;
                case 2:
                    return c;
                case 3:
                    return d;
                default:
                    __builtin_trap();
            }
        }

        constexpr const vec4& operator[] (std::size_t idx) const {
            switch (idx) {
                case 0:
                    return a;
                case 1:
                    return b;
                case 2:
                    return c;
                case 3:
                    return d;
                default:
                    __builtin_trap();
            }
        } 

        constexpr mat4 operator+ (mat4 other) const {
            return mat4(a + other.a, b + other.b, c + other.c, d + other.d);
        }

        constexpr mat4 operator- (mat4 other) const {
            return mat4(a - other.a, b - other.b, c - other.c, d - other.d);
        }

        constexpr mat4 transpose() const {
            return mat4(
                a.x, b.x, c.x, d.x,
                a.y, b.y, c.y, d.y,
                a.z, b.z, c.z, d.z,
                a.w, b.w, c.w, d.w);
        }

        constexpr mat4 operator* (mat4 m) const {
            return mat4(
                a.x * m.a.x + b.x * m.a.y + c.x * m.a.z + d.x * m.a.w,
                a.y * m.a.x + b.y * m.a.y + c.y * m.a.z + d.y * m.a.w,
                a.z * m.a.x + b.z * m.a.y + c.z * m.a.z + d.z * m.a.w,
                a.w * m.a.x + b.w * m.a.y + c.w * m.a.z + d.w * m.a.w,
                
                a.x * m.b.x + b.x * m.b.y + c.x * m.b.z + d.x * m.b.w,
                a.y * m.b.x + b.y * m.b.y + c.y * m.b.z + d.y * m.b.w,
                a.z * m.b.x + b.z * m.b.y + c.z * m.b.z + d.z * m.b.w,
                a.w * m.b.x + b.w * m.b.y + c.w * m.b.z + d.w * m.b.w,

                a.x * m.c.x + b.x * m.c.y + c.x * m.c.z + d.x * m.c.w,
                a.y * m.c.x + b.y * m.c.y + c.y * m.c.z + d.y * m.c.w,
                a.z * m.c.x + b.z * m.c.y + c.z * m.c.z + d.z * m.c.w,
                a.w * m.c.x + b.w * m.c.y + c.w * m.c.z + d.w * m.c.w,

                a.x * m.d.x + b.x * m.d.y + c.x * m.d.z + d.x * m.d.w,
                a.y * m.d.x + b.y * m.d.y + c.y * m.d.z + d.y * m.d.w,
                a.z * m.d.x + b.z * m.d.y + c.z * m.d.z + d.z * m.d.w,
                a.w * m.d.x + b.w * m.d.y + c.w * m.d.z + d.w * m.d.w);
        }

        constexpr vec4 operator* (vec4 v) const {
            return vec4(
                a.x * v.x + b.x * v.y + c.x * v.z + d.x * v.w,
                a.y * v.x + b.y * v.y + c.y * v.z + d.y * v.w,
                a.z * v.x + b.z * v.y + c.z * v.z + d.z * v.w,
                a.w * v.x + b.w * v.y + c.w * v.z + d.w * v.w);
        }  
        
        static constexpr mat4 translation(float x, float y, float z, float w) {
            return mat4(
                1.0F, 0.0F, 0.0F, 0.0F,
                0.0F, 1.0F, 0.0F, 0.0F,
                0.0F, 0.0F, 1.0F, 0.0F,
                x, y, z, w);
        }

        static constexpr mat4 translation(vec4 tv) {
            return translation(tv.x, tv.y, tv.z, tv.w);
        }

        static constexpr mat4 scale(float sx, float sy, float sz, float sw) {
            return mat4(
                sx, 0.0F, 0.0F, 0.0F,
                0.0F, sy, 0.0F, 0.0F,
                0.0F, 0.0F, sz, 0.0F,
                0.0F, 0.0F, 0.0F, sw);
        }

        static constexpr mat4 zero() {
            return scale(0.0F, 0.0F, 0.0F, 0.0F);
        }

        static constexpr mat4 identity() {
            return scale(1.0F, 1.0F, 1.0F, 1.0F);
        }

        static constexpr mat4 scale(vec4 sv) {
            return scale(sv.x, sv.y, sv.z, sv.w);
        }

        static constexpr mat4 ortho(float left, float right, float bottom, float top, float near, float far) {
            return mat4(
                2.0F / (right - left), 0.0F, 0.0F, 0.0F,
                0.0F, 2.0F / (top - bottom), 0.0F, 0.0F,
                0.0F, 0.0F, -2.0F / (far - near), 0.0F,
                -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0F);
        }

        private:
            static constexpr mat4 _perspective(float fov, float aspect, float near, float far) {
                return mat4(
                    fov / aspect, 0.0F, 0.0F, 0.0F,
                    0.0F, fov, 0.0F, 0.0F,
                    0.0F, 0.0F, (far + near) / (near - far), -1.0F,
                    0.0F, 0.0F, (2.0F * far * near) / (near - far), 0.0F);
            }

            static constexpr mat4 _rotateZ(float sa, float ca) {
                return mat4(
                    ca, sa, 0.0F, 0.0F,
                    -sa, ca, 0.0F, 0.0F,
                    0.0F, 0.0F, 1.0F, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F);
            }

            static constexpr mat4 _rotateY(float sa, float ca) {
                return mat4(
                    ca, 0.0F, -sa, 0.0F,
                    0.0F, 1.0F, 0.0F, 0.0F,
                    sa, 0.0f, ca, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F);
            }

            static constexpr mat4 _rotateX(float sa, float ca) {
                return mat4(
                    1.0F, 0.0F, 0.0F, 0.0F,
                    0.0F, ca, sa, 0.0F,
                    0.0F, -sa, ca, 0.0F,
                    0.0F, 0.0F, 0.0F, 1.0F);
            }

        public:            
            static inline mat4 perspective(float fov, float aspect, float near, float far) {
                return _perspective(std::tan(fov * 0.5F), aspect, near, far);
            }

            static inline mat4 rotateX(float angle) {
                return _rotateX(std::sin(angle), std::cos(angle));
            }

            static inline mat4 rotateY(float angle) {
                return _rotateY(std::sin(angle), std::cos(angle));
            }

            static inline mat4 rotateZ(float angle) {
                return _rotateZ(std::sin(angle), std::cos(angle));
            }

            static inline mat4 lookat(vec3 eye, vec3 center, vec3 up) {
                auto forward = (center - eye).normalize();
                auto side = vec3::cross(forward, up).normalize();                
                up = side * forward;
                
                auto result = mat4(
                    side.x, up.x, -forward.x, 0.0F,
                    side.y, up.y, -forward.y, 0.0F,
                    side.z, up.z, -forward.z, 0.0F,
                    0.0F , 0.0F, 0.0F, 1.0F);                

                auto tr = translation(-eye.x, -eye.y, -eye.z, 1.0F);

                return result * tr;
            }
    };
}