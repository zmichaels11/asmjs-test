#include "audio.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
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

        auto strVersion = std::string(alGetString(AL_VERSION));
        auto strRenderer = std::string(alGetString(AL_RENDERER));
        auto strVendor = std::string(alGetString(AL_RENDERER));

        std::cout << "[AL] OpenAL Version: " << strVersion << std::endl;
        std::cout << "[AL] OpenAL Renderer: " << strRenderer << std::endl;
        std::cout << "[AL] OpenAL Vendor: " << strVendor << std::endl;
    }

    namespace {
        context_t::context_t() {
            if ((pDevice = alcOpenDevice(nullptr)) == nullptr) {
                std::cerr << "[AL] Unable to open device!" << std::endl;
                __builtin_trap();
            }

            if ((pContext = alcCreateContext(pDevice, nullptr)) == nullptr) {
                std::cerr << "[AL] Unable to create context!" << std::endl;
                __builtin_trap();
            }

            if (!alcMakeContextCurrent(pContext)) {
                std::cerr << "[AL] Unable to make context current!" << std::endl;
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