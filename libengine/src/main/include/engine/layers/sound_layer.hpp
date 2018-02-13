#pragma once

#include <memory>

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/sound_layer_info.hpp"

#include "audio.hpp"
#include "math/mat4.hpp"


namespace engine {
    namespace layers {
        class sound_layer : public base_layer {
            std::unique_ptr<base_resources> _pResources;

            sound_layer(const sound_layer&) = delete;

            sound_layer& operator=(const sound_layer&) = delete;
        
        public:
            sound_layer(
                const context& ctx,
                const sound_layer_info& info) noexcept;

            sound_layer(sound_layer&&) = default;

            sound_layer& operator=(sound_layer&&) = default;

            virtual ~sound_layer() {}

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() noexcept;

            virtual void invalidate() noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            const sound_layer_info& getInfo() const noexcept;            

            void playSoundAt(
                std::unique_ptr<audio::sound_channel>&& snd,
                float priority,
                float pitch,
                float gain,
                float x, 
                float y, 
                float z
            ) noexcept;

            void playSoundAt(
                int soundID,
                float priority,
                float pitch,
                float gain,
                float x, 
                float y, 
                float z) noexcept;            
        };
    }
}

