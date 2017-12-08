#pragma once

#include "nk/edit_filter.hpp"
#include "nk/edit_type.hpp"

namespace nk {
    void edit_string(edit_type type, char* buffer, int* len, int max, edit_filter filter);
}