#include "engine/application.hpp"

#include <cstddef>
#include <cstring>
#include <cstdio>

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
#include <vector>

#include "engine/gui/button.hpp"
#include "engine/gui/component.hpp"
#include "engine/gui/dynamic_row_layout.hpp"
#include "engine/gui/frame.hpp"
#include "engine/gui/option_group.hpp"
#include "engine/gui/static_row_layout.hpp"

namespace {
    constexpr std::size_t MAX_VERTEX_BUFFER = 512 * 1024;
    constexpr std::size_t MAX_ELEMENT_BUFFER = 128 * 1024;
    constexpr std::size_t MAX_TEXT = 1024;
    constexpr double DOUBLE_CLICK_LO = 0.02;
    constexpr double DOUBLE_CLICK_HI = 0.2;

    void _onError(const std::string& msg) {
        std::cerr << msg << std::endl;
        __builtin_trap();
    }    

    void doFrame();

    struct native_resources {
        struct glfw_resources_T {
            GLFWwindow * pWindow;
        } glfw;

        struct openal_resources_T {
            ALCdevice * pDevice;
            ALCcontext * pContext;
        } oal;

        struct nuklear_resources_T {
            nk_context context;
            int width, height;
            int displayWidth, displayHeight;

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

            nk_font_atlas atlas;
            struct nk_vec2 fbScale;
            unsigned int text[MAX_TEXT];
            std::size_t textLen;
            struct nk_vec2 scroll;
            double lastButtonClick;
            bool isDoubleClickDown;
            struct nk_vec2 doubleClickPos;
        } nuklear;

        native_resources() {}

        native_resources(const std::string& title, unsigned int width, unsigned int height);        

        ~native_resources();    

        bool isValid() const;        

        void nkDeviceCreate();

        void nkUploadAtlas(const void * image, unsigned int width, unsigned int height);

        void nkRender(nk_anti_aliasing AA = NK_ANTI_ALIASING_OFF);

        void nkFontStashBegin(nk_font_atlas ** ppAtlas);

        void nkFontStashEnd();

        void nkNewFrame();
    };

    void nkClipboardCopy(nk_handle usr, const char * text, int len);

    void nkClipboardPaste(nk_handle usr, nk_text_edit * exit);

    void nkCharCallback(GLFWwindow * win, unsigned int codepoint);

    void nkScrollCallback(GLFWwindow * win, double xoff, double yoff);

    void nkMouseButtonCallback(GLFWwindow * win, int button, int action, int mods);    
}

namespace {
    std::unique_ptr<native_resources> _pNativeResources;
    std::function<void(void*)> _onFrame;
    std::shared_ptr<void> _pUserData;

    engine::gui::component * _pComponents;
    std::size_t _numComponents;

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

        void setGUI(gui::component * pComponents, std::size_t count) {
            _pComponents = pComponents;
            _numComponents = count;
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

    namespace gui {
        void frame::setChildren(const std::vector<std::shared_ptr<component>>& children) {
            _children = children;
        }

        void frame::build() {            
            auto pCtx = &_pNativeResources->nuklear.context;

            if (nk_begin(pCtx, _title.c_str(), nk_rect(_x, _y, _w, _h), static_cast<nk_flags> (_opts))) {
                for (auto&& child : _children) {
                    child->build();
                }
            }

            nk_end(pCtx);
        }

        void button::setOnAction(const std::function<void(const button *)>& callback) {
            _onAction = callback;
        }

        void button::build() {
            auto pCtx = &_pNativeResources->nuklear.context;

            if (nk_button_label(pCtx, _label.c_str()) && _onAction) {
                _onAction(this);
            }
        }

        void static_row_layout::build() {
            auto pCtx = &_pNativeResources->nuklear.context;

            nk_layout_row_static(pCtx, _height, _itemWidth, _cols);
        }

        void dynamic_row_layout::build() {
            auto pCtx = &_pNativeResources->nuklear.context;

            nk_layout_row_dynamic(pCtx, _height, _cols);
        }

        void option_group::build() {
            auto pCtx = &_pNativeResources->nuklear.context;
            int current = _selectedIdx;

            for (int idx = 0; idx < _pOptions.size(); idx++) {                
                if (nk_option_label(pCtx, _pOptions[idx]->label.c_str(), _selectedIdx == idx)) {
                    _selectedIdx = idx;
                }
            }

            if (current != _selectedIdx && _onChange) {
                _onChange(this);
            }
        }

        option_group::option_group(const option * pOptions, std::size_t count) {
            for (int i = 0; i < count; i++) {
                _pOptions.push_back(pOptions + i);
            }

            _onChange = nullptr;
            _selectedIdx = 0;
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

        glfwSetScrollCallback(glfw.pWindow, nkScrollCallback);
        glfwSetCharCallback(glfw.pWindow, nkCharCallback);
        glfwSetMouseButtonCallback(glfw.pWindow, nkMouseButtonCallback);

        nk_init_default(&nuklear.context, 0);

        nuklear.context.clip.copy = nkClipboardCopy;
        nuklear.context.clip.paste = nkClipboardPaste;
        nuklear.context.clip.userdata = nk_handle_ptr(nullptr);
        nuklear.lastButtonClick = 0;
        
        nkDeviceCreate();

        nuklear.isDoubleClickDown = false;
        nuklear.doubleClickPos = nk_vec2(0.0F, 0.0F);

        nk_font_atlas * pAtlas;
        nkFontStashBegin(&pAtlas);
        // add fonts now
        nkFontStashEnd();
    }

    native_resources::~native_resources() {
        if (glfw.pWindow) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(oal.pContext);
            alcCloseDevice(oal.pDevice);

            nk_font_atlas_clear(&nuklear.atlas);
            nk_free(&nuklear.context);

            glDeleteBuffers(1, &nuklear.ogl.vbo);
            glDeleteBuffers(1, &nuklear.ogl.ebo);
            glDeleteProgram(nuklear.ogl.prog);
            glDeleteTextures(1, &nuklear.ogl.fontTex);
            nk_buffer_free(&nuklear.ogl.cmds);

            glfwDestroyWindow(glfw.pWindow);
            glfwTerminate();
        }
    }

    bool native_resources::isValid() const {
        return glfw.pWindow && !glfwWindowShouldClose(glfw.pWindow);
    }

    void doFrame() {        
        glfwPollEvents();
        _time = glfwGetTime();

        if (_pComponents && _numComponents) {
            _pNativeResources->nkNewFrame();

            for (auto it = _pComponents; it != (_pComponents + _numComponents); it++) {
                it->build();
            }
        }

        _onFrame(_pUserData.get());

        if (_pComponents && _numComponents) {
            _pNativeResources->nkRender();
        }

        glfwSwapBuffers(_pNativeResources->glfw.pWindow);    
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
                "   gl_Position = ProjMtx * vec4(Position, 0, 1);\n"
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
            glGenBuffers(1, &dev.vbo);
            glGenBuffers(1, &dev.ebo);
            glGenVertexArrays(1, &dev.vao);            

            glBindVertexArray(dev.vao);                                    
            glBindBuffer(GL_ARRAY_BUFFER, dev.vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev.ebo);           

            glEnableVertexAttribArray(static_cast<GLuint> (dev.aPos));
            glEnableVertexAttribArray(static_cast<GLuint> (dev.aUV));
            glEnableVertexAttribArray(static_cast<GLuint> (dev.aCol));            

            glVertexAttribPointer(static_cast<GLuint> (dev.aPos), 2, GL_FLOAT, GL_FALSE, 20, reinterpret_cast<const void *> (0));                        
            glVertexAttribPointer(static_cast<GLuint> (dev.aUV), 2, GL_FLOAT, GL_FALSE, 20, reinterpret_cast<const void *> (8));            
            glVertexAttribPointer(static_cast<GLuint> (dev.aCol), 4, GL_UNSIGNED_BYTE, GL_TRUE, 20, reinterpret_cast<const void *> (16));
        }
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void native_resources::nkUploadAtlas(const void * image, unsigned int width, unsigned int height) {
        auto& dev = nuklear.ogl;

        if (dev.fontTex) {
            glDeleteTextures(1, &dev.fontTex);
        }

        glGenTextures(1, &dev.fontTex);
        
        glBindTexture(GL_TEXTURE_2D, dev.fontTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei> (width), static_cast<GLsizei> (height), 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }

    void native_resources::nkRender(nk_anti_aliasing AA) {        
        GLfloat ortho[] = {
            2.0f / nuklear.width, 0.0f, 0.0f, 0.0f,
            0.0f,-2.0f / nuklear.height, 0.0f, 0.0f,
            0.0f, 0.0f,-1.0f, 0.0f,
            -1.0f,1.0f, 0.0f, 1.0f};

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glActiveTexture(GL_TEXTURE0);

        auto& dev = nuklear.ogl;

        glUseProgram(dev.prog);
        glUniform1i(dev.uTex, 0);
        glUniformMatrix4fv(dev.uProj, 1, GL_FALSE, ortho);

        glViewport(0, 0, static_cast<GLsizei> (nuklear.displayWidth), static_cast<GLsizei> (nuklear.displayHeight));

        {
            auto vertices = malloc(MAX_VERTEX_BUFFER);
            auto elements = malloc(MAX_ELEMENT_BUFFER);

            {
                nk_convert_config config;
                static const nk_draw_vertex_layout_element vertexLayout[] = {
                    {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, 0},
                    {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, 8},
                    {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, 16},
                    {NK_VERTEX_LAYOUT_END}};                

                NK_MEMSET(&config, 0, sizeof(config));

                config.vertex_layout = vertexLayout;
                config.vertex_size = 20;
                config.vertex_alignment = 4;
                config.null = dev.null;
                config.circle_segment_count = 22;
                config.curve_segment_count = 22;
                config.global_alpha = 1.0F;
                config.shape_AA = AA;
                config.line_AA = AA;

                nk_buffer vbuf, ebuf;

                nk_buffer_init_fixed(&vbuf, vertices, MAX_VERTEX_BUFFER);
                nk_buffer_init_fixed(&ebuf, elements, MAX_ELEMENT_BUFFER);
                nk_convert(&nuklear.context, &dev.cmds, &vbuf, &ebuf, &config);
            }

            glBindVertexArray(dev.vao);
            glBindBuffer(GL_ARRAY_BUFFER, dev.vbo);
            glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_BUFFER, nullptr, GL_STREAM_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_VERTEX_BUFFER, vertices);            

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_ELEMENT_BUFFER, nullptr, GL_STREAM_DRAW);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, MAX_ELEMENT_BUFFER, elements);

            static bool checkOnce = true;

            if (checkOnce) {
                checkOnce = false;

                struct vtx {
                    float p[2];
                    float uv[2];
                    unsigned char col[4];
                };

                auto verts = reinterpret_cast<vtx *> (vertices);
                auto idx = reinterpret_cast<unsigned short *> (elements);
            }

            free(vertices);
            free(elements);
        }

        const nk_draw_command * cmd;
        const nk_draw_index * offset = nullptr;

        nk_draw_foreach(cmd, &nuklear.context, &dev.cmds) {
            if (!cmd->elem_count) {
                continue;
            }

            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint> (cmd->texture.id));

            auto sx = (GLint)(cmd->clip_rect.x * nuklear.fbScale.x);
			auto sy = (GLint)((nuklear.height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * nuklear.fbScale.y);
			auto sw = (GLint)(cmd->clip_rect.w * nuklear.fbScale.x);
			auto sh = (GLint)(cmd->clip_rect.h * nuklear.fbScale.y);

            glScissor(sx, sy, sw, sh);

            glDrawElements(GL_TRIANGLES, cmd->elem_count, GL_UNSIGNED_SHORT, offset);

            offset += cmd->elem_count;
        }

        if (glGetError() != GL_NO_ERROR) {
            _onError("GLERROR!");
        }

        nk_clear(&nuklear.context);

        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);
    }

    void nkCharCallback(GLFWwindow * win, unsigned int codepoint) {
        if (_pNativeResources->nuklear.textLen < MAX_TEXT) {
            _pNativeResources->nuklear.text[_pNativeResources->nuklear.textLen++] = codepoint;
        }
    }

    void nkScrollCallback(GLFWwindow * win, double xoff, double yoff) {
        _pNativeResources->nuklear.scroll.x += static_cast<float> (xoff);
        _pNativeResources->nuklear.scroll.y += static_cast<float> (yoff);
    }

    void nkMouseButtonCallback(GLFWwindow * win, int button, int action, int mods) {
        if (button != GLFW_MOUSE_BUTTON_LEFT) {
            return;
        }

        double x, y;

        glfwGetCursorPos(win, &x, &y);

        auto& nkres = _pNativeResources->nuklear;

        if (action == GLFW_PRESS) {            
            auto now = glfwGetTime();
            double dt = now - nkres.lastButtonClick;

            if (dt > DOUBLE_CLICK_LO && dt < DOUBLE_CLICK_HI) {
                nkres.isDoubleClickDown = true;
                nkres.doubleClickPos = nk_vec2(static_cast<float> (x), static_cast<float> (y));
            }

            nkres.lastButtonClick = now;
        } else {
            nkres.isDoubleClickDown = false;
        }
    }
    
    void nkClipboardPaste(nk_handle usr, nk_text_edit * edit) {
        const auto text = glfwGetClipboardString(_pNativeResources->glfw.pWindow);

        if (text) {
            nk_textedit_paste(edit, text, nk_strlen(text));
        }
    }

    void nkClipboardCopy(nk_handle usr, const char * text, int len) {
        if (!len) {
            return;
        }

        auto str = std::make_unique<char[]> (len + 1);

        std::memcpy(str.get(), text, static_cast<std::size_t> (len));

        str[len] = '\0';

        glfwSetClipboardString(_pNativeResources->glfw.pWindow, str.get());        
    }

    void native_resources::nkFontStashBegin(nk_font_atlas ** ppAtlas) {
        nk_font_atlas_init_default(&nuklear.atlas);
        nk_font_atlas_begin(&nuklear.atlas);

        *ppAtlas = &nuklear.atlas;
    }

    void native_resources::nkFontStashEnd() {
        int w, h;
        const auto image = nk_font_atlas_bake(&nuklear.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);

        nkUploadAtlas(image, static_cast<unsigned int> (w), static_cast<unsigned int> (h));
        
        nk_font_atlas_end(&nuklear.atlas, nk_handle_id(static_cast<int> (nuklear.ogl.fontTex)), &nuklear.ogl.null);

        if (nuklear.atlas.default_font) {
            nk_style_set_font(&nuklear.context, &nuklear.atlas.default_font->handle);
        }
    }

    void native_resources::nkNewFrame() {
        {
            glfwGetWindowSize(glfw.pWindow, &nuklear.width, &nuklear.height);
            glfwGetFramebufferSize(glfw.pWindow, &nuklear.displayWidth, &nuklear.displayHeight);
         
            nuklear.fbScale.x = static_cast<float> (nuklear.displayWidth) / static_cast<float> (nuklear.width);
            nuklear.fbScale.y = static_cast<float> (nuklear.displayHeight) / static_cast<float> (nuklear.height);
        }

        auto pCtx = &nuklear.context;
        auto pWin = glfw.pWindow;
        
        nk_input_begin(pCtx);

        for (decltype(nuklear.textLen) i = 0; i < nuklear.textLen; i++) {
            nk_input_unicode(pCtx, nuklear.text[i]);
        }
             
        nk_input_key(pCtx, NK_KEY_DEL, glfwGetKey(pWin, GLFW_KEY_DELETE) == GLFW_PRESS);
        nk_input_key(pCtx, NK_KEY_ENTER, glfwGetKey(pWin, GLFW_KEY_ENTER) == GLFW_PRESS);
        nk_input_key(pCtx, NK_KEY_TAB, glfwGetKey(pWin, GLFW_KEY_TAB) == GLFW_PRESS);
        nk_input_key(pCtx, NK_KEY_BACKSPACE, glfwGetKey(pWin, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
        nk_input_key(pCtx, NK_KEY_UP, glfwGetKey(pWin, GLFW_KEY_UP) == GLFW_PRESS);
        nk_input_key(pCtx, NK_KEY_DOWN, glfwGetKey(pWin, GLFW_KEY_DOWN) == GLFW_PRESS);

        {
            bool home = glfwGetKey(pWin, GLFW_KEY_HOME) == GLFW_PRESS;
            bool end = glfwGetKey(pWin, GLFW_KEY_END) == GLFW_PRESS;

            nk_input_key(pCtx, NK_KEY_TEXT_START, home);
            nk_input_key(pCtx, NK_KEY_TEXT_END, end);

            nk_input_key(pCtx, NK_KEY_SCROLL_START, home);
            nk_input_key(pCtx, NK_KEY_SCROLL_END, end);
        }                

        nk_input_key(pCtx, NK_KEY_SCROLL_DOWN, glfwGetKey(pWin, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
        nk_input_key(pCtx, NK_KEY_SCROLL_UP, glfwGetKey(pWin, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
        nk_input_key(pCtx, NK_KEY_SHIFT, glfwGetKey(pWin, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS 
                || glfwGetKey(pWin, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

        if (glfwGetKey(pWin, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(pWin, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
            nk_input_key(pCtx, NK_KEY_COPY, glfwGetKey(pWin, GLFW_KEY_C) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_PASTE, glfwGetKey(pWin, GLFW_KEY_V) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_CUT, glfwGetKey(pWin, GLFW_KEY_X) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_TEXT_UNDO, glfwGetKey(pWin, GLFW_KEY_Z) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_TEXT_REDO, glfwGetKey(pWin, GLFW_KEY_Y) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_TEXT_WORD_LEFT, glfwGetKey(pWin, GLFW_KEY_LEFT) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_TEXT_WORD_RIGHT, glfwGetKey(pWin, GLFW_KEY_RIGHT) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_TEXT_LINE_START, glfwGetKey(pWin, GLFW_KEY_B) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_TEXT_LINE_END, glfwGetKey(pWin, GLFW_KEY_E) == GLFW_PRESS);
        } else {
            nk_input_key(pCtx, NK_KEY_LEFT, glfwGetKey(pWin, GLFW_KEY_LEFT) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_RIGHT, glfwGetKey(pWin, GLFW_KEY_RIGHT) == GLFW_PRESS);
            nk_input_key(pCtx, NK_KEY_COPY, false);
            nk_input_key(pCtx, NK_KEY_PASTE, false);
            nk_input_key(pCtx, NK_KEY_CUT, false);
        }

        {
            int x, y;

            {
                double xpos, ypos;

                glfwGetCursorPos(pWin, &xpos, &ypos);
                x = static_cast<int> (xpos);
                y = static_cast<int> (ypos);
            }            

            nk_input_motion(pCtx, x, y);
            nk_input_button(pCtx, NK_BUTTON_LEFT, x, y, glfwGetMouseButton(pWin, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
            nk_input_button(pCtx, NK_BUTTON_MIDDLE, x, y, glfwGetMouseButton(pWin, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
            nk_input_button(pCtx, NK_BUTTON_RIGHT, x, y, glfwGetMouseButton(pWin, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
            nk_input_button(pCtx, NK_BUTTON_DOUBLE, static_cast<int> (nuklear.doubleClickPos.x), static_cast<int> (nuklear.doubleClickPos.y), nuklear.isDoubleClickDown);
            nk_input_scroll(pCtx, nuklear.scroll);                        
        }        

        nk_input_end(pCtx);

        nuklear.textLen = 0;
        nuklear.scroll = nk_vec2(0.0F, 0.0F);
    }
}