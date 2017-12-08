#pragma once

namespace nk {
    void property(const char* name, int min, int* pValue, int max, int step, float incPerPixel = 1.0F);

    void property(const char* name, float min, float* pValue, float max, float step, float incPerPixel = 1.0F);

    void property(const char* name, double min, double* pValue, double max, double step, float incPerPixel = 1.0F);

    int property(const char* name, int min, int value, int max, int step, float incPerPixel = 1.0F);

    float property(const char* name, float min, float value, float max, float step, float incPerPixel = 1.0F);

    double proeprty(const char* name, double min, double value, double max, double step, float incPerPixel = 1.0F);
}