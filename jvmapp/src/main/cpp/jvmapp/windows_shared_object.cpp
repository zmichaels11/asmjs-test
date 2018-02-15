#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "pch.h"
#include "jvmapp/shared_object.hpp"

#include <windows.h>

#include "jvmapp/link_exception.hpp"

namespace jvmapp {
    shared_object::shared_object(const std::string& dllPath) {        
        _handle = reinterpret_cast<void * > (LoadLibraryA(dllPath.c_str()));

        if (!_handle) {
            std::stringstream err;

            err << "[link_exception] Error linking shared object: \""
                << dllPath
                << "\"";

            throw link_exception(err.str());
        }
    }

    shared_object::~shared_object() {
        if (_handle) {
            FreeLibrary(reinterpret_cast<HMODULE> (_handle));
            _handle = nullptr;
        }
    }
}
#endif
