#include "engine/gui/text_field.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {        
        template<>
        void text_field<text_filter::DEFAULT>::build(void * ctx) {            
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_edit_string(&nk->context, NK_EDIT_FIELD, _buffer, &_filled, _bufferSize, nk_filter_default);
        }

        template<>
        void text_field<text_filter::ASCII>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_edit_string(&nk->context, NK_EDIT_FIELD, _buffer, &_filled, _bufferSize, nk_filter_ascii);
        }

        template<>
        void text_field<text_filter::FLOAT>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_edit_string(&nk->context, NK_EDIT_FIELD, _buffer, &_filled, _bufferSize, nk_filter_float);
        }

        template<>
        void text_field<text_filter::DECIMAL>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_edit_string(&nk->context, NK_EDIT_FIELD, _buffer, &_filled, _bufferSize, nk_filter_decimal);
        }

        template<>
        void text_field<text_filter::HEX>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_edit_string(&nk->context, NK_EDIT_FIELD, _buffer, &_filled, _bufferSize, nk_filter_hex);
        }

        template<>
        void text_field<text_filter::OCT>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_edit_string(&nk->context, NK_EDIT_FIELD, _buffer, &_filled, _bufferSize, nk_filter_oct);
        }
    }
}