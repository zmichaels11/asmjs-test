#include "engine/application.hpp"

#include <AL/alc.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_PRIVATE
#include "nuklear/nuklear.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace {
    void _onError(const std::string& msg) {
        std::cerr << msg << std::endl;
        __builtin_trap();
    }    

    void doFrame();

    struct vertex {
        float position[2];
        float uv[2];
        nk_byte col[4];
    };

    struct native_resources {
        struct glfw_resources_T {
            GLFWwindow * pWindow;
        } glfw;

        struct openal_resources_T {
            ALCdevice * pDevice;
            ALCcontext * pContext;
        } oal;

        struct nuklear_resources_T {
            nk_context * pContext;
            unsigned int width, heigth;
            unsigned int displayWidth, displayHeight;

            struct device_T {
                nk_buffer cmds;
                nk_draw_null_texture null;
                GLuint vbo, vao, ebo;
                GLuint prog;
                GLint aPos;
                GLint aUV;
                GLint aCol;
                GLint uTex;
                GLint uProj;
                GLuint fontTex;
            } ogl;
        } nuklear;

        native_resources() {}

        native_resources(const std::string& title, unsigned int width, unsigned int height);        

        ~native_resources();

        void update();

        bool isValid() const;

        void nkCharCallback(GLFWwindow * win, unsigned int codepoint);

        void nkScrollCallback(GLFWwindow * win, double xoff, double yoff);

        void nkMouseButtonCallback(GLFWwindow * win, int button, int action, int mods);

        void nkDeviceCreate();
    };        
}

namespace {
    std::unique_ptr<native_resources> _pNativeResources;
    std::function<void(void*)> _onFrame;
    std::shared_ptr<void> _pUserData;
    double _time;            
}

namespace engine {
    namespace application {
        void setOnFrame(const std::function<void(void*)>& callback) {
            _onFrame = callback;
        }

        void init(const std::string& name, unsigned int width, unsigned int height) {
            _pNativeResources = std::make_unique<native_resources> (name, width, height);
        }

        double getTime() {
            return _time;
        }

        void start(const std::shared_ptr<void>& pUserData) {
            _pUserData = pUserData;

#ifdef __EMSCRIPTEN__
            emscripten_set_main_loop(doFrame, 60, 1);
#else
            while (_pNativeResources->isValid()) {
                doFrame();
            }
#endif
        }
    }
}

namespace {
    native_resources::native_resources(const std::string& title, unsigned int width, unsigned int height) {
        if (!glfwInit()) {
            _onError("glfwInit failed!");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        if ((glfw.pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)) == nullptr) {
            _onError("glfwCreateWindow failed!");
        }

        glfwMakeContextCurrent(glfw.pWindow);
        glfwSwapInterval(1);

        if ((oal.pDevice = alcOpenDevice(nullptr)) == nullptr) {
            _onError("alcOpenDevice failed!");
        }

        if ((oal.pContext = alcCreateContext(oal.pDevice, nullptr)) == nullptr) {
            _onError("alcCreateContext failed!");
        }

        if (!alcMakeContextCurrent(oal.pContext)) {
            _onError("alcMakeContextCurrent failed!");
        }
    }

    native_resources::~native_resources() {        
        glDeleteBuffers(1, &nuklear.ogl.vbo);
        glDeleteBuffers(1, &nuklear.ogl.ebo);
        glDeleteProgram(nuklear.ogl.prog);
        

        if (oal.pContext && oal.pDevice) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(oal.pContext);
            alcCloseDevice(oal.pDevice);            
        }

        if (glfw.pWindow) {
            glfwDestroyWindow(glfw.pWindow);
            glfwTerminate();
        }
    }

    bool native_resources::isValid() const {
        return glfw.pWindow && !glfwWindowShouldClose(glfw.pWindow);
    }

    void native_resources::update() {
        glfwSwapBuffers(glfw.pWindow);
        glfwPollEvents();
    }    

    void doFrame() {        
        _onFrame(_pUserData.get());
        _pNativeResources->update();
        _time = glfwGetTime();
    }

    void native_resources::nkDeviceCreate() {        
        auto& dev = nuklear.ogl;

        nk_buffer_init_default(&dev.cmds);
        
        {
            static const GLchar * VERTEX_SHADER =
                "#version 300 es\n"
                "uniform mat4 ProjMtx;\n"
                "in vec2 Position;\n"
                "in vec2 TexCoord;\n"
                "in vec4 Color;\n"
                "out vec2 Frag_UV;\n"
                "out vec4 Frag_Color;\n"
                "void main() {\n"
                "   Frag_UV = TexCoord;\n"
                "   Frag_Color = Color;\n"
                "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
                "}\n";

            static const GLchar * FRAGMENT_SHADER =
                "#version 300 es\n"
                "precision mediump float;\n"
                "uniform sampler2D Texture;\n"
                "in vec2 Frag_UV;\n"
                "in vec4 Frag_Color;\n"
                "out vec4 Out_Color;\n"
                "void main(){\n"
                "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
                "}\n";

            auto vsh = glCreateShader(GL_VERTEX_SHADER);
            auto fsh = glCreateShader(GL_FRAGMENT_SHADER);

            glShaderSource(vsh, 1, &VERTEX_SHADER, nullptr);
            glShaderSource(fsh, 1, &FRAGMENT_SHADER, nullptr);

            glCompileShader(vsh);
            glCompileShader(fsh);

            GLint status;
            
            glGetShaderiv(vsh, GL_COMPILE_STATUS, &status);

            if (status != GL_TRUE) {
                _onError("Unable to compile vertex shader!");
            }

            glGetShaderiv(fsh, GL_COMPILE_STATUS, &status);

            if (status != GL_TRUE) {
                _onError("Unable to compile fragment shader!");
            }

            auto prog = glCreateProgram();

            glAttachShader(prog, vsh);
            glAttachShader(prog, fsh);

            glLinkProgram(prog);

            glGetProgramiv(prog, GL_LINK_STATUS, &status);

            if (status != GL_TRUE) {                
                _onError("Unable to link program!");                
            }

            dev.aPos = glGetAttribLocation(prog, "Position");
            dev.aUV = glGetAttribLocation(prog, "TexCoord");
            dev.aCol = glGetAttribLocation(prog, "Color");
            dev.uTex = glGetUniformLocation(prog, "Texture");
            dev.uProj = glGetUniformLocation(prog, "ProjMtx");
            dev.prog = prog;

            glDetachShader(prog, fsh);
            glDetachShader(prog, vsh);

            glDeleteShader(vsh);
            glDeleteShader(fsh);            
        }

        {
            GLsizei vs = sizeof(vertex);
            auto vp = offsetof(vertex, position);
            auto vt = offsetof(vertex, uv);
            auto vc = offsetof(vertex, col);

            glGenBuffers(1, &dev.vbo);
            glGenBuffers(1, &dev.ebo);
            glGenVertexArrays(1, &dev.vao);

            glBindVertexArray(dev.vao);
            glBindBuffer(GL_ARRAY_BUFFER, dev.vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev.vao);

            glEnableVertexAttribArray(static_cast<GLuint> (dev.aPos));
            glEnableVertexAttribArray(static_cast<GLuint> (dev.aUV));
            glEnableVertexAttribArray(static_cast<GLuint> (dev.aCol));

            glVertexAttribPointer(static_cast<GLuint> (dev.aPos), 2, GL_FLOAT, GL_FALSE, vs, reinterpret_cast<void *> (vp));
            glVertexAttribPointer(static_cast<GLuint> (dev.aUV), 2, GL_FLOAT, GL_FALSE, vs, reinterpret_cast<void *> (vt));
            glVertexAttribPointer(static_cast<GLuint> (dev.aCol), 2, GL_FLOAT, GL_FALSE, vs, reinterpret_cast<void *> (vc));
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}