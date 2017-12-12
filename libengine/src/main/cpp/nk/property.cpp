#include "nk/property.hpp"

#include "nk/nk_ctx.hpp"

namespace nk {
    void property(const char* name, int min, int* pValue, int max, int step, float incPerPixel) {
        auto ctx = nk::getContext();

        nk_property_int(&ctx->context, name, min, pValue, max, step, incPerPixel);
    }

    void property(const char* name, float min, float* pValue, float max, float step, float incPerPixel) {
        auto ctx = nk::getContext();

        nk_property_float(&ctx->context, name, min, pValue, max, step, incPerPixel);
    }

    void property(const char* name, double min, double* pValue, double max, double step, double incPerPixel) {
        auto ctx = nk::getContext();

        nk_property_double(&ctx->context, name, min, pValue, max, step, incPerPixel);
    }

    int property(const char* name, int min, int value, int max, int step, float incPerPixel) {
        auto ctx = nk::getContext();

        return nk_propertyi(&ctx->context, name, min, value, max, step, incPerPixel);
    }

    float property(const char* name, float min, float value, float max, float step, float incPerPixel) {
        auto ctx = nk::getContext();

        return nk_propertyf(&ctx->context, name, min, value, max, step, incPerPixel);
    }

    double property(const char* name, double min, double value, double max, double step, double incPerPixel) {
        auto ctx = nk::getContext();

        return nk_propertyd(&ctx->context, name, min, value, max, step, incPerPixel);
    }
}