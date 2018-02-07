#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#if defined(GLES20)
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>
#elif defined(GLES30)
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#elif defined(GL)
#include <GLFW/glfw3.h>
#else
#include <GLFW/glfw3.h>
#error "No GL specified!"
#endif

#include <cstddef>
#include <cstring>

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "audio.hpp"
#include "graphics.hpp"
#include "util.hpp"