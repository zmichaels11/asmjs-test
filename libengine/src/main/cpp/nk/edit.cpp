#include "nk/edit.hpp"

#include "nuklear/nk_ctx.hpp"

namespace nk {
    namespace {
        nk_plugin_filter _filter(edit_filter filter) {
            switch (filter) {
                case edit_filter::ASCII:
                    return nk_filter_ascii;
                case edit_filter::BINARY:
                    return nk_filter_binary;
                case edit_filter::DECIMAL:
                    return nk_filter_decimal;
                case edit_filter::FLOAT:
                    return nk_filter_float;
                case edit_filter::HEX:
                    return nk_filter_hex;
                case edit_filter::OCT:
                    return nk_filter_oct;
                default:
                    return nk_filter_default;
            }
        }
    }

    void edit_string(edit_type type, char* buffer, int* len, int max, edit_filter filter) {
        auto ctx = nk::getContext();
        
        nk_edit_string(&ctx->context, static_cast<nk_flags> (type), buffer, len, max, _filter(filter));
    }
}