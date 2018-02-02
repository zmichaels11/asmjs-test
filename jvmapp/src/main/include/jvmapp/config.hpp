#pragma once

#include <cstddef>

#include <string>

namespace jvmapp {
    const std::string JAVA_MAIN = "jvmapp/Demo";
    const std::string JAVA_VM_ARGS[] = {
        "-Xms256M",
        "-Dapp.name=Demo"
    };

    constexpr auto JAVA_VM_ARG_COUNT = sizeof(JAVA_VM_ARGS) / sizeof(std::string);
}