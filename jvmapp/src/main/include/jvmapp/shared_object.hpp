#pragma once

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "jvmapp/load_exception.hpp"

namespace jvmapp {
    class shared_object {
        void * _handle;

        shared_object(const shared_object&) = delete;

        shared_object& operator=(const shared_object&) = delete;

    public:
        inline shared_object(shared_object&& other) noexcept {
            _handle = other._handle;
            other._handle = nullptr;
        }

        inline shared_object& operator= (shared_object&& other) {
            _handle = other._handle;
            other._handle = nullptr;

            return *this;
        }

        shared_object(const std::string& dllPath);

        shared_object() noexcept:
            _handle(nullptr) {}

        ~shared_object();

        template<class T>
        std::function<T> loadPfn(const std::string& fnName) const;
    };
}

#if defined(__linux__)
#include <dlfcn.h>

namespace jvmapp {
    template<class T>
    std::function<T> shared_object::loadPfn(const std::string& fnName) const {
        auto result = dlsym(_handle, fnName.c_str());

        if (!result) {
            std::stringstream err;

            err << "[load_exception] Error: could not load function symbol \""
                << fnName
                << "\"";

            {
                auto errstr = dlerror();

                if (errstr) {
                    err << "; " << errstr;
                }
            }

            throw load_exception(err.str());
        }

        return reinterpret_cast<T*> (result);
    }
}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
namespace jvmapp {
    template<class T>
    std::function<T> shared_object::loadPfn(const std::string& fnName) const {
        auto result = GetProcAddress(
            reinterpret_cast<HMODULE> (_handle),
            fnName.c_str());

        if (!result) {
            std::stringstream err;

            err << "[load_exception] Error: could not load function symbol\""
                << fnName
                << "\"";

            throw load_exception(err.str());
        }

        return reinterpret_cast<T*> (result);
    }
}

#else
#error "Only linux and windows are supported right now!"
#endif
