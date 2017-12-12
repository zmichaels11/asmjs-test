#pragma once

#include <cstddef>

#include <memory>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear/nuklear.hpp"

namespace nk {
    constexpr std::size_t MAX_TEXT = 1024;

    struct nk_ctx;

    nk_ctx * getContext();

    struct nk_ctx {        
        bool requestNewFrame;
        nk_context context;        

        struct size_t {
            int width, height;
            int displayWidth, displayHeight;
            float scaleW, scaleH;
        } size;        
        
        struct input_t {
            struct scroll_t {
                float x, y;
            } scroll;
            
            struct double_click_t {
                float x, y;
                bool isDown;
            } doubleClick;

            double lastButtonClick;
        } input;

        struct device_t {
            nk_buffer cmds;
            nk_draw_null_texture null;
            nk_font_atlas atlas;
            
            struct gl_objs_t {
                struct buffers_t {
                    unsigned int vbo;
                    unsigned int ebo;
                } buffers;

                unsigned int vao;
                unsigned int program;
                unsigned int fontTexture;

                struct attrib_t {
                    int position;
                    int texCoord;
                    int color;
                } attributes;

                struct uniform_t {
                    int texture;
                    int projection;
                } uniforms;
            } gl;

            unsigned int text[MAX_TEXT];
            std::size_t textLen;
        } device;

        nk_ctx(void *);

        ~nk_ctx();

        void render();

        void renderPrevious();

        void newFrame();
    };
}