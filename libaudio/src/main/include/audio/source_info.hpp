#pragma once

namespace audio {
    struct source_info {
        float gain;
        float pitch;
        struct position_t {
            float x, y, z;

            position_t() noexcept:
                x(0.0F),
                y(0.0F),
                z(0.0F) {}

            position_t(float tx, float ty, float tz) noexcept:
                x(tx),
                y(ty),
                z(tz) {}

        } position;

        source_info() noexcept:
            gain(1.0F),
            pitch(1.0F),
            position() {}

        source_info(
            float tgain,
            float tpitch,
            float tx,
            float ty,
            float tz) noexcept:
        
            gain(tgain),
            pitch(tpitch),
            position(tx, ty, tz) {}
    };
}