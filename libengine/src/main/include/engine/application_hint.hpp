#pragma once

namespace engine {
    enum class application_hint : unsigned int {
        VSYNC = 0x1,
        DEBUG = 0x2,
    };

    constexpr application_hint operator| (application_hint lhs, application_hint rhs) {
        return static_cast<application_hint> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr application_hint operator& (application_hint lhs, application_hint rhs) {
        return static_cast<application_hint> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }

    constexpr application_hint& operator|= (application_hint& lhs, application_hint rhs) {
        return lhs = lhs | rhs;
    }

    constexpr application_hint& operator&= (application_hint& lhs, application_hint rhs) {
        return lhs = lhs & rhs;
    }    
}