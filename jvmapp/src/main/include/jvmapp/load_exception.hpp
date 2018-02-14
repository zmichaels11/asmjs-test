#pragma once

#include <exception>
#include <string>

namespace jvmapp {
    class load_exception : public std::exception {
        std::string _msg;

    public:
        load_exception(const std::string& msg = "load_exception") noexcept:
            _msg(msg) {}

        virtual const char * what() const throw();
    };
}