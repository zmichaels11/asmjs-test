#pragma once

#include <functional>
#include <sstream>
#include <string>

#include "jvmapp/load_exception.hpp"

namespace jvmapp {
    class shared_object {
        void * _handle;

        shared_object(const shared_object&) = delete;

        shared_object& operator=(const shared_object&) = delete;

    public:
        shared_object(shared_object&&) = default;

        shared_object& operator= (shared_object&&) = default;

        shared_object(const std::string& dllPath);

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
#error "Windows implement not completed yet!"
#else
#error "Only linux and windows are supported right now!"
#endif