#include "audio.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <cstdio>

#include <memory>

namespace audio {
    namespace {
        struct context_t {
            ALCdevice * pDevice;
            ALCcontext * pContext;

            context_t();

            ~context_t();
        };

        std::unique_ptr<context_t> _context;
    }

    void init() noexcept {
        _context = std::make_unique<context_t>();

        std::printf("[AL] OpenAL Version: %s\n", alGetString(AL_VERSION));
        std::printf("[AL] OpenAL Renderer: %s\n", alGetString(AL_RENDERER));
        std::printf("[AL] OpenAL Vendor: %s\n", alGetString(AL_VENDOR));
    }

    namespace {
        context_t::context_t() {
            if ((pDevice = alcOpenDevice(nullptr)) == nullptr) {
                std::printf("[AL] Unable to open device!\n");
                __builtin_trap();
            }

            if ((pContext = alcCreateContext(pDevice, nullptr)) == nullptr) {
                std::printf("[AL] Unable to create context!\n");
                __builtin_trap();
            }

            if (!alcMakeContextCurrent(pContext)) {
                std::printf("[AL] Unable to make context current!\n");
                __builtin_trap();
            }
        }

        context_t::~context_t() {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(pContext);
            alcCloseDevice(pDevice);
        }
    }
}