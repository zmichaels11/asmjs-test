#include "pch.h"
#include "jvmapp/load_exception.hpp"

namespace jvmapp {
    const char * load_exception::what() const throw() {
        return _msg.c_str();
    }
}