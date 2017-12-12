#include "nuklear/nk_ctx.hpp"

#include <cstddef>
#include <cstring>

#include <iostream>
#include <memory>
#include <string>

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
#include "nuklear/nuklear.hpp"

namespace nk {
    namespace {
        constexpr double DOUBLE_CLICK_LO = 0.02;
        constexpr double DOUBLE_CLICK_HI = 0.2;
        constexpr nk_anti_aliasing AA = NK_ANTI_ALIASING_OFF;
        constexpr std::size_t MAX_VERTEX_BUFFER = 512 * 1024;
        constexpr std::size_t MAX_ELEMENT_BUFFER = 128 * 1024;

        static const nk_draw_vertex_layout_element VERTEX_LAYOUT[] = {
            {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, 0},
            {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, 8},
            {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, 16},
            {NK_VERTEX_LAYOUT_END}};

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

        nk_ctx * _pIMPL;
        GLFWwindow * _pWIN;

        void _onError(const std::string& msg);

        void _nkClipboardCopy(nk_handle usr, const char * text, int len);

        void _nkClipboardPaste(nk_handle user, nk_text_edit * edit);

        void _nkCharCallback(GLFWwindow * win, unsigned int codepoint);

        void _nkScrollCallback(GLFWwindow * win, double xOff, double yOff);

        void _nkMouseButtonCallback(GLFWwindow * win, int button, int action, int mods);

        void _nkCreateDevice();

        void _nkFontStashBegin(nk_font_atlas ** ppAtlas);

        void _nkFontStashEnd();

        void _nkUploadAtlas(const void * image, int width, int height);
    }

    nk_ctx::nk_ctx(void * ptr) {
        _pIMPL = this;
        _pWIN = reinterpret_cast<GLFWwindow *> (ptr);

        glfwSetScrollCallback(_pWIN, _nkScrollCallback);
        glfwSetCharCallback(_pWIN, _nkCharCallback);
        glfwSetMouseButtonCallback(_pWIN, _nkMouseButtonCallback);

        nk_init_default(&context, 0);

        context.clip.copy = _nkClipboardCopy;
        context.clip.paste = _nkClipboardPaste;
        context.clip.userdata = nk_handle_ptr(nullptr);
        input.lastButtonClick = 0.0;

        _nkCreateDevice();

        input.doubleClick = {0.0F, 0.0F, false};
        
        nk_font_atlas * pAtlas;
        _nkFontStashBegin(&pAtlas);
        //TODO: add fonts here
        _nkFontStashEnd();
    }

    nk_ctx::~nk_ctx() {
        nk_font_atlas_clear(&device.atlas);
        nk_free(&context);
        nk_buffer_free(&device.cmds);

        glDeleteBuffers(1, &device.gl.buffers.vbo);
        glDeleteBuffers(1, &device.gl.buffers.ebo);

        glDeleteVertexArrays(1, &device.gl.vao);

        glDeleteProgram(device.gl.program);

        glDeleteTextures(1, &device.gl.fontTexture);

        _pWIN = nullptr;
        _pIMPL = nullptr;        
    }

    void nk_ctx::newFrame() {
        glfwGetWindowSize(_pWIN, &size.width, &size.height);
        glfwGetFramebufferSize(_pWIN, &size.displayWidth, &size.displayHeight);

        size.scaleW = static_cast<float> (size.displayWidth) / static_cast<float> (size.width);
        size.scaleH = static_cast<float> (size.displayHeight) / static_cast<float> (size.height);

        nk_input_begin(&context);

        for (decltype(device.textLen) i = 0; i < device.textLen; i++) {
            nk_input_unicode(&context, device.text[i]);
        }

        nk_input_key(&context, NK_KEY_DEL, glfwGetKey(_pWIN, GLFW_KEY_DELETE) == GLFW_PRESS);
        nk_input_key(&context, NK_KEY_ENTER, glfwGetKey(_pWIN, GLFW_KEY_ENTER) == GLFW_PRESS);
        nk_input_key(&context, NK_KEY_TAB, glfwGetKey(_pWIN, GLFW_KEY_TAB) == GLFW_PRESS);
        nk_input_key(&context, NK_KEY_BACKSPACE, glfwGetKey(_pWIN, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
        nk_input_key(&context, NK_KEY_UP, glfwGetKey(_pWIN, GLFW_KEY_UP) == GLFW_PRESS);
        nk_input_key(&context, NK_KEY_DOWN, glfwGetKey(_pWIN, GLFW_KEY_DOWN) == GLFW_PRESS);

        {
            bool home = glfwGetKey(_pWIN, GLFW_KEY_HOME) == GLFW_PRESS;
            bool end = glfwGetKey(_pWIN, GLFW_KEY_END) == GLFW_PRESS;

            nk_input_key(&context, NK_KEY_TEXT_START, home);
            nk_input_key(&context, NK_KEY_TEXT_END, end);

            nk_input_key(&context, NK_KEY_SCROLL_START, home);
            nk_input_key(&context, NK_KEY_SCROLL_END, end);
        }                

        nk_input_key(&context, NK_KEY_SCROLL_DOWN, glfwGetKey(_pWIN, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
        nk_input_key(&context, NK_KEY_SCROLL_UP, glfwGetKey(_pWIN, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
        nk_input_key(&context, NK_KEY_SHIFT, glfwGetKey(_pWIN, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS 
                || glfwGetKey(_pWIN, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

        if (glfwGetKey(_pWIN, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(_pWIN, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
            nk_input_key(&context, NK_KEY_COPY, glfwGetKey(_pWIN, GLFW_KEY_C) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_PASTE, glfwGetKey(_pWIN, GLFW_KEY_V) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_CUT, glfwGetKey(_pWIN, GLFW_KEY_X) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_TEXT_UNDO, glfwGetKey(_pWIN, GLFW_KEY_Z) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_TEXT_REDO, glfwGetKey(_pWIN, GLFW_KEY_Y) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_TEXT_WORD_LEFT, glfwGetKey(_pWIN, GLFW_KEY_LEFT) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_TEXT_WORD_RIGHT, glfwGetKey(_pWIN, GLFW_KEY_RIGHT) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_TEXT_LINE_START, glfwGetKey(_pWIN, GLFW_KEY_B) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_TEXT_LINE_END, glfwGetKey(_pWIN, GLFW_KEY_E) == GLFW_PRESS);
        } else {
            nk_input_key(&context, NK_KEY_LEFT, glfwGetKey(_pWIN, GLFW_KEY_LEFT) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_RIGHT, glfwGetKey(_pWIN, GLFW_KEY_RIGHT) == GLFW_PRESS);
            nk_input_key(&context, NK_KEY_COPY, false);
            nk_input_key(&context, NK_KEY_PASTE, false);
            nk_input_key(&context, NK_KEY_CUT, false);
        }

        {
            int x, y;

            {
                double xpos, ypos;

                glfwGetCursorPos(_pWIN, &xpos, &ypos);
                x = static_cast<int> (xpos);
                y = static_cast<int> (ypos);
            }            

            nk_input_motion(&context, x, y);
            nk_input_button(&context, NK_BUTTON_LEFT, x, y, glfwGetMouseButton(_pWIN, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
            nk_input_button(&context, NK_BUTTON_MIDDLE, x, y, glfwGetMouseButton(_pWIN, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
            nk_input_button(&context, NK_BUTTON_RIGHT, x, y, glfwGetMouseButton(_pWIN, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
            nk_input_button(&context, NK_BUTTON_DOUBLE, static_cast<int> (input.doubleClick.x), static_cast<int> (input.doubleClick.y), input.doubleClick.isDown);
            nk_input_scroll(&context, nk_vec2(input.scroll.x, input.scroll.y));
        }        

        nk_input_end(&_pIMPL->context);

        device.textLen = 0;
        input.scroll = {0.0F, 0.0F};
    }

    void nk_ctx::render() {
        GLfloat ortho[] = {
            2.0f / size.width, 0.0f, 0.0f, 0.0f,
            0.0f,-2.0f / size.height, 0.0f, 0.0f,
            0.0f, 0.0f,-1.0f, 0.0f,
            -1.0f,1.0f, 0.0f, 1.0f
        };

        //TODO: switch to premultiplied alpha
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glActiveTexture(GL_TEXTURE0);

        glUseProgram(device.gl.program);
        glUniform1i(device.gl.uniforms.texture, 0);
        glUniformMatrix4fv(device.gl.uniforms.projection, 1, GL_FALSE, ortho);

        glViewport(0, 0, static_cast<GLsizei> (size.displayWidth), static_cast<GLsizei> (size.displayHeight));

        {
            auto vertices = std::make_unique<GLchar[]> (MAX_VERTEX_BUFFER);
            auto elements = std::make_unique<GLchar[]> (MAX_ELEMENT_BUFFER);

            {
                nk_convert_config config;
                
                NK_MEMSET(&config, 0, sizeof(config));

                config.vertex_layout = VERTEX_LAYOUT;
                config.vertex_size = 20;
                config.vertex_alignment = 4;
                config.null = device.null;
                config.circle_segment_count = 22;
                config.curve_segment_count = 22;
                config.global_alpha = 1.0F;
                config.shape_AA = AA;
                config.line_AA = AA;

                nk_buffer vbuf, ebuf;

                nk_buffer_init_fixed(&vbuf, vertices.get(), MAX_VERTEX_BUFFER);
                nk_buffer_init_fixed(&ebuf, elements.get(), MAX_ELEMENT_BUFFER);
                nk_convert(&context, &device.cmds, &vbuf, &ebuf, &config);
            }

            glBindVertexArray(device.gl.vao);
        
            glBindBuffer(GL_ARRAY_BUFFER, device.gl.buffers.vbo);
            glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_BUFFER, nullptr, GL_STREAM_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_VERTEX_BUFFER, vertices.get());

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, device.gl.buffers.ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_ELEMENT_BUFFER, nullptr, GL_STREAM_DRAW);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, MAX_ELEMENT_BUFFER, elements.get());    
        }            

        const nk_draw_command * cmd;
        const nk_draw_index * offset = nullptr;

        nk_draw_foreach(cmd, &context, &device.cmds) {
            if (!cmd->elem_count) {
                continue;
            }

            glBindTexture(GL_TEXTURE_2D, static_cast<GLuint> (cmd->texture.id));                

            auto sx = (GLint) (cmd->clip_rect.x * size.scaleW);
            auto sy = (GLint) ((size.height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * size.scaleH);
            auto sw = (GLint) (cmd->clip_rect.w * size.scaleW);
            auto sh = (GLint) (cmd->clip_rect.h * size.scaleH);

            glScissor(sx, sy, sw, sh);
            glDrawElements(GL_TRIANGLES, cmd->elem_count, GL_UNSIGNED_SHORT, offset);
            offset += cmd->elem_count;
        }

        nk_clear(&context);

        glDisable(GL_BLEND);
        glDisable(GL_SCISSOR_TEST);
    }

    namespace {
        void _nkCreateDevice() {
            nk_buffer_init_default(&_pIMPL->device.cmds);

            {
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

                glDetachShader(prog, fsh);
                glDetachShader(prog, vsh);

                glDeleteShader(fsh);
                glDeleteShader(vsh);

                glGetProgramiv(prog, GL_LINK_STATUS, &status);

                if (status != GL_TRUE) {
                    _onError("Unable to link program!");
                }

                if ((_pIMPL->device.gl.attributes.position = glGetAttribLocation(prog, "Position")) < 0) {
                    _onError("Unable to find position vertex attribute");
                }

                if ((_pIMPL->device.gl.attributes.texCoord = glGetAttribLocation(prog, "TexCoord")) < 0) {
                    _onError("Unable to find texcoord vertex attribute!");
                }

                if ((_pIMPL->device.gl.attributes.color = glGetAttribLocation(prog, "Color")) < 0) {
                    _onError("Unable to find color vertex attribute!");
                }

                if ((_pIMPL->device.gl.uniforms.texture = glGetUniformLocation(prog, "Texture")) < 0) {
                    _onError("Unable to find texture uniform!");
                }

                if ((_pIMPL->device.gl.uniforms.projection = glGetUniformLocation(prog, "ProjMtx")) < 0) {
                    _onError("Unable to find projection matrix uniform!");
                }
                
                _pIMPL->device.gl.program = prog;
            }

            {
                GLuint vbo, ebo, vao;

                glGenBuffers(1, &vbo);
                glGenBuffers(1, &ebo);
                glGenVertexArrays(1, &vao);

                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

                glEnableVertexAttribArray(static_cast<GLuint> (_pIMPL->device.gl.attributes.position));
                glEnableVertexAttribArray(static_cast<GLuint> (_pIMPL->device.gl.attributes.texCoord));
                glEnableVertexAttribArray(static_cast<GLuint> (_pIMPL->device.gl.attributes.color));

                glVertexAttribPointer(static_cast<GLuint> (_pIMPL->device.gl.attributes.position), 2, GL_FLOAT, GL_FALSE, 20, reinterpret_cast<const void *> (0));
                glVertexAttribPointer(static_cast<GLuint> (_pIMPL->device.gl.attributes.texCoord), 2, GL_FLOAT, GL_FALSE, 20, reinterpret_cast<const void *> (8));
                glVertexAttribPointer(static_cast<GLuint> (_pIMPL->device.gl.attributes.color), 4, GL_UNSIGNED_BYTE, GL_TRUE, 20, reinterpret_cast<const void *> (16));

                if (glGetError() != GL_NO_ERROR) {
                    _onError("Error preparing Vertex Array Object!");
                }

                _pIMPL->device.gl.buffers.vbo = vbo;
                _pIMPL->device.gl.buffers.ebo = ebo;
                _pIMPL->device.gl.vao = vao;

                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }          
        }

        void _onError(const std::string& msg) {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }

        void _nkUploadAtlas(const void * image, int width, int height) {
            if (_pIMPL->device.gl.fontTexture) {
                glDeleteTextures(1, &_pIMPL->device.gl.fontTexture);
            }

            glGenTextures(1, &_pIMPL->device.gl.fontTexture);

            glBindTexture(GL_TEXTURE_2D, _pIMPL->device.gl.fontTexture);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);            

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei> (width), static_cast<GLsizei> (height), 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

            glBindTexture(GL_TEXTURE_2D, 0);

            if (glGetError() != GL_NO_ERROR) {
                _onError("Error preparing Font Atlas!");
            }
        }

        void _nkFontStashBegin(nk_font_atlas ** ppAtlas) {
            nk_font_atlas_init_default(&_pIMPL->device.atlas);
            nk_font_atlas_begin(&_pIMPL->device.atlas);

            *ppAtlas = &_pIMPL->device.atlas;
        }

        void _nkFontStashEnd() {
            int w, h;
            const auto image = nk_font_atlas_bake(&_pIMPL->device.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);                        

            _nkUploadAtlas(image, w, h);

            nk_font_atlas_end(&_pIMPL->device.atlas, nk_handle_id((int)_pIMPL->device.gl.fontTexture), &_pIMPL->device.null);  

            if (_pIMPL->device.atlas.default_font) {                
                nk_style_set_font(&_pIMPL->context, &_pIMPL->device.atlas.default_font->handle);
            }
        }

        void _nkClipboardCopy(nk_handle usr, const char * text, int len) {
            if (!len) {
                return;
            }

            auto str = std::make_unique<char[]> (len + 1);

            std::memcpy(str.get(), text, static_cast<std::size_t> (len));

            str[len] = '\0';

            glfwSetClipboardString(_pWIN, str.get());
        }

        void _nkClipboardPaste(nk_handle user, nk_text_edit * edit) {
            const auto text = glfwGetClipboardString(_pWIN);

            if (text) {
                nk_textedit_paste(edit, text, nk_strlen(text));
            }
        }

        void _nkCharCallback(GLFWwindow * win, unsigned int codepoint) {
            if (_pIMPL->device.textLen < MAX_TEXT) {
                _pIMPL->device.text[_pIMPL->device.textLen++] = codepoint;
            }
        }

        void _nkScrollCallback(GLFWwindow * win, double xOff, double yOff) {
            _pIMPL->input.scroll.x = static_cast<float> (xOff);
            _pIMPL->input.scroll.y = static_cast<float> (yOff);
        }

        void _nkMouseButtonCallback(GLFWwindow * win, int button, int action, int mods) {
            if (button != GLFW_MOUSE_BUTTON_LEFT) {
                return;
            }

            double x, y;

            glfwGetCursorPos(win, &x, &y);

            if (action == GLFW_PRESS) {
                auto now = glfwGetTime();
                auto dt = now - _pIMPL->input.lastButtonClick;

                if (dt > DOUBLE_CLICK_LO && dt < DOUBLE_CLICK_HI) {
                    _pIMPL->input.doubleClick = {
                        static_cast<float> (x), static_cast<float> (y),
                        true};
                }

                _pIMPL->input.lastButtonClick = now;
            } else {
                _pIMPL->input.doubleClick.isDown = false;
            }
        }
    }
}