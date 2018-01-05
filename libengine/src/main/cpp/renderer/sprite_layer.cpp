#include "renderer/sprite_layer.hpp"

#include <vector>

namespace renderer {
    namespace {
        struct vertex {

        };

        struct sprite_layer_res_impl : public virtual sprite_layer_res {            
            virtual ~sprite_layer_res_impl() {}

            float projection[16];
            std::vector<vertex> vertices;            
        };
    }

    const sprite_layer_info& sprite_layer::getInfo() const {
        return _info;
    }

    const scissor_rect& sprite_layer::getScissor() const {
        return _scissor;
    }
}