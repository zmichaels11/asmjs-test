#ifdef GLES30

#include "graphics/hobject/vertex_array.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <string>

#include "graphics/hinfo/vertex_attribute_description.hpp"
#include "graphics/hinfo/vertex_array_info.hpp"
#include "graphics/hinfo/vertex_binding_description.hpp"
#include "graphics/hobject/buffer.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;

        GLenum _type(vertex_format format) noexcept;

        bool _normalized(vertex_format format) noexcept;

        GLint _size(vertex_format format) noexcept;

        GLsizei _bytes(vertex_format format) noexcept;

        vertex_binding_description * _findBinding(const vertex_array_info& info, unsigned int binding) noexcept;
    }

    vertex_array::vertex_array(const vertex_array_info& info) noexcept {
        _info = info;
        _external = false;
        _handle = 0;
        
        glGenVertexArrays(1, &_handle);
        glBindVertexArray(_handle);

        if (info.pIndexBuffer) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.pIndexBuffer->_handle);
        }

        for (int i = 0; i < info.attributeCount; i++) {
            auto pCurrent = info.ppAttributes[i];            
            auto pBinding = _findBinding(info, pCurrent->binding);

            glBindBuffer(GL_ARRAY_BUFFER, pBinding->buffer->_handle);
            glEnableVertexAttribArray(pCurrent->location);

            auto strideAdjust = pBinding->stride;
            auto size = _size(pCurrent->format);
            auto type = _type(pCurrent->format);
            auto normalized = _normalized(pCurrent->format);
            auto offset = reinterpret_cast<const void *> (pBinding->offset + pCurrent->offset);

            if (pBinding->divisor) {
                glVertexAttribDivisor(pCurrent->location, pBinding->divisor);
            }

            glVertexAttribPointer(pCurrent->location, size, type, normalized, strideAdjust, offset);
        }
    }

    vertex_array::~vertex_array() noexcept {
        if (_handle && !_external) {
            glDeleteVertexArrays(1, &_handle);
            _handle = 0;
        }
    }

    void vertex_array::bind() const noexcept {
        glBindVertexArray(_handle);
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }

        bool _normalized(vertex_format format) noexcept {
            switch (format) {
                case vertex_format::X8_UNORM:
                case vertex_format::X8_SNORM:
                case vertex_format::X16_UNORM:
                case vertex_format::X16_SNORM:
                case vertex_format::X8Y8_UNORM:
                case vertex_format::X8Y8_SNORM:
                case vertex_format::X16Y16_UNORM:
                case vertex_format::X16Y16_SNORM:
                case vertex_format::X8Y8Z8_UNORM:
                case vertex_format::X8Y8Z8_SNORM:
                case vertex_format::X16Y16Z16_UNORM:
                case vertex_format::X16Y16Z16_SNORM:
                case vertex_format::X8Y8Z8W8_UNORM:
                case vertex_format::X8Y8Z8W8_SNORM:
                case vertex_format::X16Y16Z16W16_UNORM:
                case vertex_format::X16Y16Z16W16_SNORM:
                case vertex_format::W2Z10Y10X10_UNORM:
                case vertex_format::W2Z10Y10X10_SNORM: 
                    return true;                
                default:
                    return false;
            }
        }

        GLenum _type(vertex_format format) noexcept {
            switch (format) {
                case vertex_format::X8_UNORM:
                case vertex_format::X8Y8_UNORM:
                case vertex_format::X8Y8Z8_UNORM:
                case vertex_format::X8Y8Z8W8_UNORM:
                case vertex_format::X8_UINT:
                case vertex_format::X8Y8_UINT:
                case vertex_format::X8Y8Z8_UINT:
                case vertex_format::X8Y8Z8W8_UINT:               
                    return GL_UNSIGNED_BYTE;
                case vertex_format::X8_SNORM:
                case vertex_format::X8Y8_SNORM:
                case vertex_format::X8Y8Z8_SNORM:
                case vertex_format::X8Y8Z8W8_SNORM:
                case vertex_format::X8_SINT:
                case vertex_format::X8Y8_SINT:
                case vertex_format::X8Y8Z8_SINT:
                case vertex_format::X8Y8Z8W8_SINT:
                    return GL_BYTE;
                case vertex_format::X16_UNORM:
                case vertex_format::X16Y16_UNORM:
                case vertex_format::X16Y16Z16_UNORM:
                case vertex_format::X16Y16Z16W16_UNORM:
                case vertex_format::X16_UINT:
                case vertex_format::X16Y16_UINT:
                case vertex_format::X16Y16Z16_UINT:
                case vertex_format::X16Y16Z16W16_UINT:
                    return GL_UNSIGNED_SHORT;
                case vertex_format::X16_SNORM:
                case vertex_format::X16Y16_SNORM:
                case vertex_format::X16Y16Z16_SNORM:
                case vertex_format::X16Y16Z16W16_SNORM:
                case vertex_format::X16_SINT:
                case vertex_format::X16Y16_SINT:
                case vertex_format::X16Y16Z16_SINT:
                case vertex_format::X16Y16Z16W16_SINT:
                    return GL_SHORT;
                case vertex_format::X32_UINT:
                case vertex_format::X32Y32_UINT:
                case vertex_format::X32Y32Z32_UINT:
                case vertex_format::X32Y32Z32W32_UINT:
                    return GL_UNSIGNED_INT;
                case vertex_format::X32_SINT:
                case vertex_format::X32Y32_SINT:
                case vertex_format::X32Y32Z32_SINT:
                case vertex_format::X32Y32Z32W32_SINT:
                    return GL_INT;
                case vertex_format::X16_SFLOAT:
                case vertex_format::X16Y16_SFLOAT:
                case vertex_format::X16Y16Z16_SFLOAT:
                case vertex_format::X16Y16Z16W16_SFLOAT:
                    return GL_HALF_FLOAT;
                case vertex_format::X32_SFLOAT:
                case vertex_format::X32Y32_SFLOAT:
                case vertex_format::X32Y32Z32_SFLOAT:
                case vertex_format::X32Y32Z32W32_SFLOAT:
                    return GL_FLOAT;
                case vertex_format::W2Z10Y10X10_SNORM:
                    return GL_INT_2_10_10_10_REV;
                case vertex_format::W2Z10Y10X10_UNORM:
                    return GL_UNSIGNED_INT_2_10_10_10_REV;
                default:
                    _onError("Unsupported vertx format!");
            }
        }

        GLint _size(vertex_format format) noexcept {
            switch (format) {                
                case vertex_format::X8Y8Z8W8_SINT:
                case vertex_format::X8Y8Z8W8_SNORM:
                case vertex_format::X8Y8Z8W8_UINT:
                case vertex_format::X8Y8Z8W8_UNORM:                    
                case vertex_format::X16Y16Z16W16_SFLOAT:
                case vertex_format::X16Y16Z16W16_SINT:
                case vertex_format::X16Y16Z16W16_SNORM:
                case vertex_format::X16Y16Z16W16_UNORM:
                case vertex_format::X16Y16Z16W16_UINT:                                                    
                case vertex_format::X32Y32Z32W32_SFLOAT:                
                case vertex_format::X32Y32Z32W32_SINT:  
                case vertex_format::X32Y32Z32W32_UINT:                
                case vertex_format::W2Z10Y10X10_SNORM:
                case vertex_format::W2Z10Y10X10_UNORM:                
                    return 4;
                    
                case vertex_format::X8Y8Z8_UNORM:
                case vertex_format::X8Y8Z8_SNORM:
                case vertex_format::X8Y8Z8_UINT:
                case vertex_format::X8Y8Z8_SINT:                    
                case vertex_format::X16Y16Z16_SFLOAT:
                case vertex_format::X16Y16Z16_SINT:
                case vertex_format::X16Y16Z16_SNORM:
                case vertex_format::X16Y16Z16_UINT:
                case vertex_format::X16Y16Z16_UNORM:   
                case vertex_format::X32Y32Z32_SFLOAT:
                case vertex_format::X32Y32Z32_SINT:
                case vertex_format::X32Y32Z32_UINT:             
                    return 3;
                
                case vertex_format::X8Y8_SINT:
                case vertex_format::X8Y8_SNORM:
                case vertex_format::X8Y8_UINT:
                case vertex_format::X8Y8_UNORM:
                case vertex_format::X16Y16_SFLOAT:
                case vertex_format::X16Y16_SINT:
                case vertex_format::X16Y16_SNORM:
                case vertex_format::X16Y16_UINT:
                case vertex_format::X16Y16_UNORM:
                case vertex_format::X32Y32_SFLOAT:
                case vertex_format::X32Y32_SINT:
                case vertex_format::X32Y32_UINT:
                    return 2;

                case vertex_format::X8_SINT:
                case vertex_format::X8_SNORM:
                case vertex_format::X8_UINT:
                case vertex_format::X8_UNORM:
                case vertex_format::X16_SFLOAT:
                case vertex_format::X16_SINT:
                case vertex_format::X16_SNORM:
                case vertex_format::X16_UINT:
                case vertex_format::X16_UNORM:
                case vertex_format::X32_SFLOAT:
                case vertex_format::X32_SINT:
                case vertex_format::X32_UINT:                
                    return 1;

                default:
                    _onError("Unsupported vertex format!");
            }
        }

        GLsizei _bytes(vertex_format format) noexcept {
            switch (format) {
                case vertex_format::X8_SINT:
                case vertex_format::X8_SNORM:
                case vertex_format::X8_UINT:
                case vertex_format::X8_UNORM:
                    return 1;

                case vertex_format::X8Y8_SINT:
                case vertex_format::X8Y8_SNORM:
                case vertex_format::X8Y8_UINT:
                case vertex_format::X8Y8_UNORM:
                case vertex_format::X16_SFLOAT:
                case vertex_format::X16_SINT:
                case vertex_format::X16_SNORM:
                case vertex_format::X16_UINT:
                case vertex_format::X16_UNORM:
                    return 2;

                case vertex_format::X8Y8Z8_SINT:
                case vertex_format::X8Y8Z8_SNORM:
                case vertex_format::X8Y8Z8_UINT:
                case vertex_format::X8Y8Z8_UNORM:
                    return 3;

                case vertex_format::X8Y8Z8W8_SINT:
                case vertex_format::X8Y8Z8W8_SNORM:
                case vertex_format::X8Y8Z8W8_UINT:
                case vertex_format::X8Y8Z8W8_UNORM:
                case vertex_format::X16Y16_SFLOAT:
                case vertex_format::X16Y16_SINT:
                case vertex_format::X16Y16_SNORM:
                case vertex_format::X16Y16_UINT:
                case vertex_format::X16Y16_UNORM:
                case vertex_format::X32_SFLOAT:
                case vertex_format::X32_SINT:
                case vertex_format::X32_UINT:
                case vertex_format::W2Z10Y10X10_SNORM:
                case vertex_format::W2Z10Y10X10_UNORM:                
                    return 4;

                case vertex_format::X16Y16Z16_SFLOAT:
                case vertex_format::X16Y16Z16_SINT:
                case vertex_format::X16Y16Z16_SNORM:
                case vertex_format::X16Y16Z16_UINT:
                case vertex_format::X16Y16Z16_UNORM:
                    return 6;

                case vertex_format::X16Y16Z16W16_SFLOAT:
                case vertex_format::X16Y16Z16W16_SINT:
                case vertex_format::X16Y16Z16W16_SNORM:
                case vertex_format::X16Y16Z16W16_UINT:
                case vertex_format::X16Y16Z16W16_UNORM:
                case vertex_format::X32Y32_SFLOAT:
                case vertex_format::X32Y32_SINT:
                case vertex_format::X32Y32_UINT:
                    return 8;
                
                case vertex_format::X32Y32Z32_SFLOAT:
                case vertex_format::X32Y32Z32_SINT:
                case vertex_format::X32Y32Z32_UINT:
                    return 12;

                case vertex_format::X32Y32Z32W32_SFLOAT:
                case vertex_format::X32Y32Z32W32_SINT:
                case vertex_format::X32Y32Z32W32_UINT:
                    return 16;
                                    
                default:
                    _onError("Unsupported vertex format!");
            }
        }

        vertex_binding_description * _findBinding(const vertex_array_info& info, unsigned int binding) noexcept {
            for (int i = 0; i < info.bindingCount; i++) {
                auto pCurrent = info.ppBindings[i];

                if (pCurrent && pCurrent->binding == binding) {
                    return pCurrent;
                }
            }            

            return nullptr;
        }
    }
}

#endif