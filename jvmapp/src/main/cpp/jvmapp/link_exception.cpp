#include "pch.h"
#include "jvmapp/link_exception.hpp"

namespace jvmapp {
    const char * link_exception::what() const throw() {
        return _msg.c_str();
    }
}