#if defined(__linux__)
#include "pch.h"
#include "jvmapp/shared_object.hpp"

#include <dlfcn.h>

#include "jvmapp/link_exception.hpp"

namespace jvmapp {
    shared_object::shared_object(const std::string& soPath) {
        dlerror();

        _handle = dlopen(soPath.c_str(), RTLD_LAZY);

        if (!_handle) {                        
            std::stringstream err;

            err << "[link_exception] Error linking shared object: "
                << soPath;

            {
                auto errstr = dlerror();

                if (errstr) {
                    err << "; " << errstr;
                }
            }

            throw link_exception(err.str());            
        }        
    }

    shared_object::~shared_object() {
        if (_handle) {
            dlclose(_handle);
            _handle = nullptr;
        }
    }    
}


#endif