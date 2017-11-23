#include "graphics/vertex_array.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <string>

#include "graphics/buffer.hpp"
#include "graphics/vertex_attribute_description.hpp"
#include "graphics/vertex_array_info.hpp"
#include "graphics/vertex_binding_description.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }

        static GLenum _type(vertex_format format) {
            switch (format) {
                case vertex_format::FLOAT:                    
                case vertex_format::VEC2:
                case vertex_format::VEC3:
                case vertex_format::VEC4:
                    return GL_FLOAT;
                default:
                    _onError("Unsupported vertx format!");
            }
        }

        static GLint _size(vertex_format format) {
            switch (format) {
                case vertex_format::FLOAT:
                    return 1;
                case vertex_format::VEC2:
                    return 2;
                case vertex_format::VEC3:
                    return 3;
                case vertex_format::VEC4:
                    return 4;
                default:
                    _onError("Unsupported vertex format!");
            }
        }

        static GLsizei _bytes(vertex_format format) {
            switch (format) {
                case vertex_format::FLOAT:
                    return 4;
                case vertex_format::VEC2:
                    return 8;
                case vertex_format::VEC3:
                    return 12;
                case vertex_format::VEC4:
                    return 16;
                default:
                    _onError("Unsupported vertex format!");
            }
        }

        static vertex_binding_description * _findBinding(const vertex_array_info& info, unsigned int binding) {
            for (auto it = info.pBindings; it != (info.pBindings + info.bindingCount); it++) {
                if (it->binding == binding) {
                    return it;
                }
            }

            return nullptr;
        }
    }

    vertex_array::vertex_array(const vertex_array_info& info) {
        glGenVertexArrays(1, &_handle);

        glBindVertexArray(_handle);

        if (info.pIndexBuffer) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.pIndexBuffer->_handle);
        }

        for (auto it = info.pAttributes; it != (info.pAttributes + info.attributeCount); it++) {
            vertex_binding_description * pBinding = _findBinding(info, it->binding);

            glBindBuffer(GL_ARRAY_BUFFER, pBinding->buffer->_handle);
            glEnableVertexAttribArray(it->location);

            auto strideAdjust = pBinding->stride - _bytes(it->format);

            glVertexAttribPointer(it->location, _size(it->format), _type(it->format), false, strideAdjust, reinterpret_cast<const void *> (pBinding->offset));
        }
    }

    vertex_array::~vertex_array() {
        glDeleteVertexArrays(1, &_handle);
    }

    void vertex_array::bind() const {
        glBindVertexArray(_handle);
    }
}