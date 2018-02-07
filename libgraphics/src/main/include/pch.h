#pragma once

#if defined (GL)
#include <GL/glew.h>
#elif defined (GLES30)
#include <GLES3/gl31.h>
#elif defined (GLES20)
#include <GLES2/gl2.h>
#else 
#error "No GL defined!"
#endif

#include <cstddef>
#include <cstring>

#include <iostream>
#include <memory>
#include <string>

#include "util.hpp"