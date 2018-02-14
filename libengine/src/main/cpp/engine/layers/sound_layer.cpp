#include "pch.h"
#include "engine/layers/sound_layer.hpp"

#include <vector>

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            struct sound_t {         
                std::unique_ptr<audio::sound_channel> _channel;                
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

                } _position;
                float _priority;
                bool _looping;
                bool _eof;
                bool _positional;

                sound_t() noexcept:
                    _channel(nullptr),
                    _position(),
                    _priority(0.0F),
                    _looping(false),
                    _eof(true),
                    _positional(false) {}

                sound_t(
                    std::unique_ptr<audio::sound_channel>&& channel, 
                    float priority, 
                    bool looping, 
                    bool positional, 
                    float x, float y, float z) noexcept:

                    _channel(std::move(channel)),
                    _position(x, y, z),
                    _priority(priority),
                    _looping(looping),                                        
                    _eof(false),
                    _positional(positional) {}
            };

            struct sound_layer_resources : public base_resources {
                const context * _pCtx;
                sound_layer_info _info;
                float _projection[16];

                std::vector<audio::source> _sources;
                std::vector<sound_t> _sounds;

                sound_layer_resources(
                    const context& ctx,
                    const sound_layer_info& info) noexcept;

                virtual ~sound_layer_resources() {}
            };            
        }

        sound_layer::sound_layer(
            const context& ctx,
            const sound_layer_info& info) noexcept {

            _pResources = std::make_unique<sound_layer_resources> (ctx, info);
        }

        const sound_layer_info& sound_layer::getInfo() const noexcept {
            auto pRes = dynamic_cast<sound_layer_resources * > (_pResources.get());

            return pRes->_info;
        }

        void sound_layer::playSoundAt(int soundID, float priority, float pitch, float gain, float x, float y, float z) noexcept {
            auto pRes = dynamic_cast<sound_layer_resources * > (_pResources.get()); 
            auto snd = pRes->_pCtx->openSoundChannel(soundID);

            playSoundAt(std::move(snd), priority, pitch, gain, x, y, z);
        }

        void sound_layer::playSoundAt(
            std::unique_ptr<audio::sound_channel>&& snd,
            float priority,
            float pitch, float gain,
            float x,float y, float z) noexcept {

            auto pRes = dynamic_cast<sound_layer_resources * > (_pResources.get());

            auto source = audio::source(audio::source_info(gain, pitch, x, y, z));
            auto sound = sound_t(std::move(snd), priority, false, true, x, y, z);

            constexpr std::size_t BUFFER_SIZE = 4096;

            float secondsPerBuffer = static_cast<float> (BUFFER_SIZE) / static_cast<float> (sound._channel->getByteRate());
            float bufferTime = secondsPerBuffer;
            int initialBuffers = 1;

            while (bufferTime < 0.1F) {
                initialBuffers++;
                bufferTime += secondsPerBuffer;
            }                   

            for (int i = 0; i < initialBuffers; i++) {
                std::size_t size = BUFFER_SIZE;
                auto transfer = std::make_unique<char[]> (size);
                bool eof = !sound._channel->read(transfer.get(), size);                

                audio::buffer buffer;

                buffer.setData(
                    sound._channel->getFormat(), 
                    transfer.get(), 
                    size, 
                    sound._channel->getSampleRate());

                source.queueBuffer(std::move(buffer));

                //TODO: support small sounds
                if (eof) {
                    break;
                }
            }
            
            source.play();
            
            pRes->_sources.push_back(std::move(source));
            pRes->_sounds.push_back(std::move(sound));
        }

        void sound_layer::beginWrite() noexcept {
            auto pRes = dynamic_cast<sound_layer_resources * > (_pResources.get());

            auto preserveSources = std::vector<audio::source>();
            auto preserveSounds = std::vector<sound_t>();
            auto nSounds = pRes->_sources.size();

            preserveSources.reserve(nSounds);

            for (decltype(nSounds) i = 0; i < nSounds; i++) {
                auto tmpSource = std::move(pRes->_sources[i]);
                auto tmpSound = std::move(pRes->_sounds[i]);

                if (tmpSource.getState() != audio::source_state::STOPPED) {                    
                    preserveSources.push_back(std::move(tmpSource));
                    preserveSounds.push_back(std::move(tmpSound));
                } else {
                    // clear any lingering buffers.
                    tmpSource.unqueueBuffers();                    
                }
            }
            
            std::swap(pRes->_sources, preserveSources);
            std::swap(pRes->_sounds, preserveSounds);
        }

        void sound_layer::endWrite() noexcept {
            //TODO: remove low priority sounds

            auto pRes = dynamic_cast<sound_layer_resources * > (_pResources.get());
            auto nSounds = pRes->_sources.size();

            for (decltype(nSounds) i = 0; i < nSounds; i++) {
                auto& source = pRes->_sources[i];            
                auto require = source.unqueueBuffers();

                if (require == 0) {
                    continue;
                }

                auto& sound = pRes->_sounds[i];

                if (sound._channel == nullptr) {
                    // channel is closed; sound is ending
                    continue;
                }

                for (decltype(require) j = 0; j < require; j++) {
                    std::size_t size = 4096;
                    auto transfer = std::make_unique<char[]> (size);

                    if (sound._looping) {

                    } else {                                                
                        sound._eof = !sound._channel->read(transfer.get(), size);

                        audio::buffer buffer;

                        buffer.setData(sound._channel->getFormat(), transfer.get(), size, sound._channel->getSampleRate());

                        source.queueBuffer(std::move(buffer));

                        if (sound._eof) {
                            sound._channel = nullptr;
                            break;
                        }
                    }
                }
            }
        }

        void sound_layer::render() noexcept {
            
        }

        void sound_layer::invalidate() noexcept {

        }

        void sound_layer::setProjection(const math::mat4& projection) noexcept {
            auto pRes = dynamic_cast<sound_layer_resources * > (_pResources.get());

            projection.data(pRes->_projection);

            for (decltype(pRes->_sounds.size()) i = 0; i < pRes->_sounds.size(); i++) {
                auto& sound = pRes->_sounds[i];
                auto& source = pRes->_sources[i];

                if (sound._positional) {
                    auto newPos = projection * math::vec4(sound._position.x, sound._position.y, 0.0, 1.0);

                    source.setPosition(newPos.x, newPos.y, newPos.z);
                }                
            }
        }

        void sound_layer::setProjection(const float * projection) noexcept {
            auto m = math::mat4(
                math::vec4(projection[0], projection[1], projection[2], projection[3]),
                math::vec4(projection[4], projection[5], projection[6], projection[7]),
                math::vec4(projection[8], projection[9], projection[10], projection[11]),
                math::vec4(projection[12], projection[13], projection[14], projection[15]));
                
            setProjection(m);
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::cout << "[render engine] sound_layer error: " << msg << std::endl;
                __builtin_trap();
            }

            sound_layer_resources::sound_layer_resources(
                const context& ctx,
                const sound_layer_info& info) noexcept {

                _info = info;
                _pCtx = &ctx;
                
                _projection[0] = 1.0F;
                _projection[1] = 0.0F;
                _projection[2] = 0.0F;
                _projection[3] = 0.0F;
                _projection[4] = 0.0F;
                _projection[5] = 1.0F;
                _projection[6] = 0.0F;
                _projection[7] = 0.0F;
                _projection[8] = 0.0F;
                _projection[9] = 0.0F;
                _projection[10] = 1.0F;
                _projection[11] = 0.0F;
                _projection[12] = 0.0F;
                _projection[13] = 0.0F;
                _projection[14] = 0.0F;
                _projection[15] = 1.0F;
            }
        }
    }
}