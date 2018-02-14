#pragma once

#include <exception>
#include <string>

namespace jvmapp {
    class link_exception : public std::exception {
        std::string _msg;

    public:
        link_exception(const std::string& msg = "link_exception was thrown!") noexcept:
            _msg(msg) {}

        virtual const char* what() const throw();
    };
}