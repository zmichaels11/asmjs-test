#include "graphics/henum/internal_format.hpp"

#include "graphics/henum/pixel_format.hpp"
#include "graphics/henum/pixel_type.hpp"

namespace graphics {
     internal_format join(pixel_format fmt, pixel_type type) {
        // handle packed types
        switch (type) {
            case pixel_type::UNSIGNED_SHORT_5_6_5:
                return internal_format::RGB565;
            case pixel_type::UNSIGNED_SHORT_4_4_4_4:
                return internal_format::RGBA4;
            case pixel_type::UNSIGNED_SHORT_5_5_5_1:
                return internal_format::RGB5_A1;
            case pixel_type::UNSIGNED_INT_2_10_10_10_REV:
                return internal_format::RGB10_A2;
            default:
                // not a packed type; handle next
                break;
        }

        // hande integer types
        switch (fmt) {
            case pixel_format::RED_INTEGER:
            case pixel_format::RG_INTEGER:
            case pixel_format::RGB_INTEGER:
            case pixel_format::RGBA_INTEGER:
            default:
                // not an integer type; handle next
                break;
        }

        // handle floating point types
        switch (type) {
            case pixel_type::HALF_FLOAT:
                switch (fmt) {
                    case pixel_format::RED:
                        return internal_format::R16F;
                    case pixel_format::RG:
                        return internal_format::RG16F;
                    case pixel_format::RGB:
                        return internal_format::RGB16F;
                    case pixel_format::RGBA:
                        return internal_format::RGBA16F;
                    default:
                        return internal_format::UNKNOWN;
                }
            case pixel_type::FLOAT:
                switch (fmt) {
                    case pixel_format::RED:
                        return internal_format::R32F;
                    case pixel_format::RG:
                        return internal_format::RG32F;
                    case pixel_format::RGB:
                        return internal_format::RGB32F;
                    case pixel_format::RGBA:
                        return internal_format::RGBA32F;
                    default:
                        return internal_format::UNKNOWN;
                }
            default:
                // not a floating point type; handle next
                break;
        }

        // handle normalized types
        switch (fmt) {
            case pixel_format::RED:
                switch (type) {
                    case pixel_type::UNSIGNED_BYTE:
                        return internal_format::R8;
                    case pixel_type::BYTE:
                        return internal_format::R8_SNORM;
                    case pixel_type::UNSIGNED_SHORT:
                        return internal_format::R16;
                    case pixel_type::SHORT:
                        return internal_format::R16_SNORM;
                    default:
                        return internal_format::UNKNOWN;
                }
            case pixel_format::RG:
                switch (type) {
                    case pixel_type::UNSIGNED_BYTE:
                        return internal_format::RG8;
                    case pixel_type::BYTE:
                        return internal_format::RG8_SNORM;
                    case pixel_type::UNSIGNED_SHORT:
                        return internal_format::RG16;
                    case pixel_type::SHORT:
                        return internal_format::RG16_SNORM;
                    default:
                        return internal_format::UNKNOWN;
                }
            case pixel_format::RGB:
                switch (type) {
                    case pixel_type::UNSIGNED_BYTE:
                        return internal_format::RGB8;
                    case pixel_type::BYTE:
                        return internal_format::RGB8_SNORM;
                    case pixel_type::UNSIGNED_SHORT:
                        return internal_format::RGB16;
                    case pixel_type::SHORT:
                        return internal_format::RGB16_SNORM;
                    default:
                        return internal_format::UNKNOWN;
                }
            case pixel_format::RGBA:
                switch (type) {
                    case pixel_type::UNSIGNED_BYTE:
                        return internal_format::RGBA8;
                    case pixel_type::BYTE:
                        return internal_format::RGBA8_SNORM;
                    case pixel_type::UNSIGNED_SHORT:
                        return internal_format::RGBA16;
                    case pixel_type::SHORT:
                        return internal_format::RGBA16_SNORM;
                    default:
                        return internal_format::UNKNOWN;
                }
            default:
                return internal_format::UNKNOWN;
        }
    }
}