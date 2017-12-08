#pragma once

namespace nk {
    class button_label {
        bool _success;

    public:
        button_label(const char * title);

        explicit operator bool() const;
    };
}