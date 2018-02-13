#include "pch.h"
#include "engine/layers/gui_layer.hpp"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"

#include "math/mat4.hpp"

#include "engine/application.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/gui_layer_info.hpp"

namespace engine {
    namespace layers {
        namespace {
            constexpr std::size_t MAX_TEXT_SIZE = 1024;
            constexpr double DOUBLE_CLICK_LO = 0.02;
            constexpr double DOUBLE_CLICK_HI = 0.2;
            constexpr nk_anti_aliasing AA = NK_ANTI_ALIASING_OFF;
            constexpr std::size_t MAX_VERTEX_BUFFER_SIZE = 512 * 1024;
            constexpr std::size_t MAX_ELEMENT_BUFFER_SIZE = 124 * 1024;

            const nk_draw_vertex_layout_element VERTEX_LAYOUT[] = {
                {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, 0},
                {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, 8},
                {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, 16},
                {NK_VERTEX_LAYOUT_END}};            

            struct gui_layer_resources : public base_resources {
                gui_layer_info _info;
                bool _requestNewFrame;
                nk_context _context;
                std::vector<nk_font * > _fonts;
                
                struct size_t {
                    int width, height;
                    int displayWidth, displayHeight;
                    float scaleW, scaleH;
                } _size;

                struct input_t {
                    struct scroll_t {
                        float x, y;
                    } scroll;

                    struct double_click_t {
                        float x, y;
                        bool isDown;
                    } doubleClick;

                    double lastButtonClick;
                } _input;

                struct device_t {
                    nk_buffer cmds;
                    nk_draw_null_texture null;
                    nk_font_atlas atlas;

                    struct gl_objects_t {
                        graphics::buffer vbo;
                        graphics::buffer ebo;
                        graphics::vertex_array vao;
                        graphics::texture fontTexture;
                        graphics::texture nullTexture;
                    } gl;
                    
                    unsigned int text[MAX_TEXT_SIZE];
                    std::size_t textLen;
                } _device;                

                gui_layer_resources(
                    const context& ctx,
                    const gui_layer_info& info) noexcept;

                virtual ~gui_layer_resources();

                bool handleScrollEvent(double sx, double sy) noexcept;

                bool handleCharEvent(unsigned int codepoint) noexcept;

                bool handleMouseButtonEvent(int button, int action, int mods) noexcept;

                void handleClipboardCopy(nk_handle usr, const char * text, int len) noexcept;

                void handleClipboardPaste(nk_handle usr, nk_text_edit * edit) noexcept;

                void bind() noexcept;
            };

            const std::string BASE_SHADER_PATH = "data/shaders/nuklear/";
#if defined(GL)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "330_core.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "330_core.frag";
#elif defined(GLES30)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "300_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "300_es.frag";
#elif defined(GLES20)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "100_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "100_es.frag";
#else
#error "No GL defined!"
#endif

            graphics::program _program;

            int _uTexture;
            int _uProjection;

            void _onError(const std::string& msg) noexcept;
        }

        gui_layer::gui_layer(
            const context& ctx,
            const gui_layer_info& info) noexcept {

            auto ptr = std::make_unique<gui_layer_resources> (ctx, info);
            
            ptr->bind();
            _pResources = std::move(ptr);
        }

        void gui_layer::beginWrite() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pWin = reinterpret_cast<GLFWwindow * > (application::getContext());            
            
            glfwGetWindowSize(pWin, &pRes->_size.width, &pRes->_size.height);
            glfwGetFramebufferSize(pWin, &pRes->_size.displayWidth, &pRes->_size.displayHeight);

            pRes->_size.scaleW = static_cast<float> (pRes->_size.displayWidth) / static_cast<float> (pRes->_size.width);
            pRes->_size.scaleH = static_cast<float> (pRes->_size.displayHeight) / static_cast<float> (pRes->_size.height);

            nk_input_begin(&pRes->_context);

            for (decltype(pRes->_device.textLen) i = 0; i < pRes->_device.textLen; i++) {
                nk_input_unicode(&pRes->_context, pRes->_device.text[i]);
            }

            nk_input_key(&pRes->_context, NK_KEY_DEL, glfwGetKey(pWin, GLFW_KEY_DELETE) == GLFW_PRESS);
            nk_input_key(&pRes->_context, NK_KEY_ENTER, glfwGetKey(pWin, GLFW_KEY_ENTER) == GLFW_PRESS);
            nk_input_key(&pRes->_context, NK_KEY_TAB, glfwGetKey(pWin, GLFW_KEY_TAB) == GLFW_PRESS);
            nk_input_key(&pRes->_context, NK_KEY_BACKSPACE, glfwGetKey(pWin, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
            nk_input_key(&pRes->_context, NK_KEY_UP, glfwGetKey(pWin, GLFW_KEY_UP) == GLFW_PRESS);
            nk_input_key(&pRes->_context, NK_KEY_DOWN, glfwGetKey(pWin, GLFW_KEY_DOWN) == GLFW_PRESS);

            {
                auto home = glfwGetKey(pWin, GLFW_KEY_HOME) == GLFW_PRESS;
                auto end = glfwGetKey(pWin, GLFW_KEY_END) == GLFW_PRESS;

                nk_input_key(&pRes->_context, NK_KEY_TEXT_START, home);
                nk_input_key(&pRes->_context, NK_KEY_TEXT_END, end);

                nk_input_key(&pRes->_context, NK_KEY_SCROLL_START, home);
                nk_input_key(&pRes->_context, NK_KEY_SCROLL_END, end);
            }                

            nk_input_key(&pRes->_context, NK_KEY_SCROLL_DOWN, glfwGetKey(pWin, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
            nk_input_key(&pRes->_context, NK_KEY_SCROLL_UP, glfwGetKey(pWin, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
            nk_input_key(&pRes->_context, NK_KEY_SHIFT, glfwGetKey(pWin, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS 
                || glfwGetKey(pWin, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

            if (glfwGetKey(pWin, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(pWin, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
                nk_input_key(&pRes->_context, NK_KEY_COPY, glfwGetKey(pWin, GLFW_KEY_C) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_PASTE, glfwGetKey(pWin, GLFW_KEY_V) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_CUT, glfwGetKey(pWin, GLFW_KEY_X) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_TEXT_UNDO, glfwGetKey(pWin, GLFW_KEY_Z) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_TEXT_REDO, glfwGetKey(pWin, GLFW_KEY_Y) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_TEXT_WORD_LEFT, glfwGetKey(pWin, GLFW_KEY_LEFT) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_TEXT_WORD_RIGHT, glfwGetKey(pWin, GLFW_KEY_RIGHT) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_TEXT_LINE_START, glfwGetKey(pWin, GLFW_KEY_B) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_TEXT_LINE_END, glfwGetKey(pWin, GLFW_KEY_E) == GLFW_PRESS);
            } else {
                nk_input_key(&pRes->_context, NK_KEY_LEFT, glfwGetKey(pWin, GLFW_KEY_LEFT) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_RIGHT, glfwGetKey(pWin, GLFW_KEY_RIGHT) == GLFW_PRESS);
                nk_input_key(&pRes->_context, NK_KEY_COPY, false);
                nk_input_key(&pRes->_context, NK_KEY_PASTE, false);
                nk_input_key(&pRes->_context, NK_KEY_CUT, false);
            }

            {
                int x, y;

                {
                    double xpos, ypos;

                    glfwGetCursorPos(pWin, &xpos, &ypos);
                    
                    x = static_cast<decltype(x)> (xpos);
                    y = static_cast<decltype(y)> (ypos);
                }            

                nk_input_motion(&pRes->_context, x, y);
                nk_input_button(&pRes->_context, NK_BUTTON_LEFT, x, y, glfwGetMouseButton(pWin, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
                nk_input_button(&pRes->_context, NK_BUTTON_MIDDLE, x, y, glfwGetMouseButton(pWin, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
                nk_input_button(&pRes->_context, NK_BUTTON_RIGHT, x, y, glfwGetMouseButton(pWin, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
                nk_input_button(&pRes->_context, NK_BUTTON_DOUBLE, static_cast<int> (pRes->_input.doubleClick.x), static_cast<int> (pRes->_input.doubleClick.y), pRes->_input.doubleClick.isDown);
                nk_input_scroll(&pRes->_context, nk_vec2(pRes->_input.scroll.x, pRes->_input.scroll.y));
            }        

            nk_input_end(&pRes->_context);

            pRes->_device.textLen = 0;
            pRes->_input.scroll = {0.0F, 0.0F};
        }

        void gui_layer::endWrite() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            
            float ortho[] = {
                2.0F / pRes->_size.width, 0.0F, 0.0F, 0.0F,
                0.0F, -2.0F / pRes->_size.height, 0.0F, 0.0F,
                0.0F, 0.0F, -1.0F, 0.0F,
                -1.0F, 1.0F, 0.0F, 1.0F};

            {
                static auto rasterStateInfo = graphics::rasterization_state_info{graphics::front_face::CCW, 1.0F, graphics::cull_mode::NONE, {false, 1.0F, 1.0F}, graphics::polygon_mode::FILL};
                static auto depthStateInfo = graphics::depth_stencil_state_info{false};

                graphics::apply(rasterStateInfo);
                graphics::apply(depthStateInfo);
            }

            _program.use();

            graphics::uniform::setUniform1(_uTexture, 0);
            graphics::uniform::setUniformMatrix4(_uProjection, 1, ortho);

            graphics::apply(graphics::viewport_state_info{0, 0, pRes->_size.displayWidth, pRes->_size.displayHeight});

            {                
                auto pVertices = std::make_unique<char[]> (MAX_VERTEX_BUFFER_SIZE);
                auto pElements = std::make_unique<char[]> (MAX_ELEMENT_BUFFER_SIZE);                

                {
                    nk_convert_config config;

                    NK_MEMSET(&config, 0, sizeof(config));

                    config.vertex_layout = VERTEX_LAYOUT;
                    config.vertex_size = 20;
                    config.vertex_alignment = 4;
                    config.null = pRes->_device.null;
                    config.circle_segment_count = 22;
                    config.curve_segment_count = 22;
                    config.global_alpha = 1.0F;
                    config.shape_AA = AA;
                    config.line_AA = AA;

                    nk_buffer vbuf, ebuf;

                    nk_buffer_init_fixed(&vbuf, pVertices.get(), MAX_VERTEX_BUFFER_SIZE);
                    nk_buffer_init_fixed(&ebuf, pElements.get(), MAX_ELEMENT_BUFFER_SIZE);

                    nk_convert(&pRes->_context, &pRes->_device.cmds, &vbuf, &ebuf, &config);
                }

                pRes->_device.gl.vbo.invalidate();
                pRes->_device.gl.vbo.subData(0, pVertices.get(), MAX_VERTEX_BUFFER_SIZE);                
                pRes->_device.gl.ebo.invalidate();
                pRes->_device.gl.ebo.subData(0, pElements.get(), MAX_ELEMENT_BUFFER_SIZE);                
            }            
        }

        void gui_layer::render() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            pRes->_device.gl.vao.bind();

            const nk_draw_command * cmd;
            nk_draw_index * offset = nullptr;

            int prevTexId = 0;

            nk_draw_foreach(cmd, &pRes->_context, &pRes->_device.cmds) {
                if (cmd->elem_count > 0) {                                        
                    if (cmd->texture.id != prevTexId) {
                        prevTexId = cmd->texture.id;
                        
                        auto texture = graphics::texture(prevTexId, graphics::texture_target::TEXTURE_2D);

                        texture.bind(0);                        
                    }                    

                    auto sx = static_cast<int>(cmd->clip_rect.x * pRes->_size.scaleW);
                    auto sy = static_cast<int>((pRes->_size.height - static_cast<int>(cmd->clip_rect.y + cmd->clip_rect.h)) * pRes->_size.scaleH);
                    auto sw = static_cast<int>(cmd->clip_rect.w * pRes->_size.scaleW);
                    auto sh = static_cast<int>(cmd->clip_rect.h * pRes->_size.scaleH);

                    auto scissorState = graphics::scissor_state_info{true, sx, sy, sw, sh};
                                    
                    graphics::apply(scissorState);
                    graphics::draw::elements(graphics::draw_mode::TRIANGLES, static_cast<GLsizei> (cmd->elem_count), graphics::index_type::UNSIGNED_SHORT, offset);

                    offset += cmd->elem_count;
                }
            }            

            graphics::apply(graphics::scissor_state_info::defaults());
            nk_clear(&pRes->_context);
            
        }

        void gui_layer::invalidate() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_clear(&pRes->_context);
        }

        void gui_layer::setProjection(const math::mat4& projection) noexcept {

        }

        void gui_layer::setProjection(const float * projection) noexcept {

        }

        bool gui_layer::begin(
            const std::string& title,
            const bounds<float, float>& bounds,
            nuklear::panel_flags flags) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_begin(&pRes->_context, title.c_str(), nk_rect(bounds.x, bounds.y, bounds.width, bounds.height), static_cast<unsigned int> (flags));
        }

        void gui_layer::end() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_end(&pRes->_context);
        }

        void gui_layer::layoutSetMinRowHeight(float height) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_set_min_row_height(&pRes->_context, height);
        }

        void gui_layer::layoutResetMinRowHeight() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_reset_min_row_height(&pRes->_context);
        }

        void gui_layer::layoutWidgetBounds() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_widget_bounds(&pRes->_context);
        }

        void gui_layer::layoutRatioFromPixel(float pixelWidth) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_ratio_from_pixel(&pRes->_context, pixelWidth);
        }        

        void gui_layer::layoutRowDynamic(
            float height,
            int cols) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_dynamic(&pRes->_context, height, cols);
        }

        void gui_layer::layoutRowStatic(
            float height,
            int itemWidth,
            int cols) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_static(&pRes->_context, height, itemWidth, cols);
        }

        template<>
        void gui_layer::layoutRowBegin<nuklear::layout_format::DYNAMIC>(
            float rowHeight,
            int cols) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_begin(&pRes->_context, NK_DYNAMIC, rowHeight, cols);
        }

        template<>
        void gui_layer::layoutRow<nuklear::layout_format::DYNAMIC>(
            float height,
            int cols,
            const float * pRatio) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row(&pRes->_context, NK_DYNAMIC, height, cols, pRatio);
        }

        template<>
        void gui_layer::layoutRow<nuklear::layout_format::STATIC>(
            float height,
            int cols,
            const float * pRatio) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row(&pRes->_context, NK_STATIC, height, cols, pRatio);
        }

        void gui_layer::layoutRowTemplateBegin(float rowHeight) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_template_begin(&pRes->_context, rowHeight);
        }

        void gui_layer::layoutRowTemplatePushDynamic() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_template_push_dynamic(&pRes->_context);
        }

        void gui_layer::layoutRowTemplatePushStatic(float width) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_template_push_static(&pRes->_context, width);
        }

        void gui_layer::layoutRowTemplatePushVariable(float minWidth) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_template_push_variable(&pRes->_context, minWidth);
        }

        void gui_layer::layoutRowTemplateEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_row_template_end(&pRes->_context);
        }

        template<>
        void gui_layer::layoutSpaceBegin<nuklear::layout_format::DYNAMIC>(
            float height,
            int widgetCount) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_space_begin(&pRes->_context, NK_DYNAMIC, height, widgetCount);
        }

        template<>
        void gui_layer::layoutSpaceBegin<nuklear::layout_format::STATIC>(
            float height,
            int widgetCount) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_space_begin(&pRes->_context, NK_STATIC, height, widgetCount);
        }

        void gui_layer::layoutSpacePush(const bounds<float, float>& rect) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_space_push(&pRes->_context, nk_rect(rect.x, rect.y, rect.width, rect.height));
        }

        void gui_layer::layoutSpaceEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_layout_space_end(&pRes->_context);
        }

        bool gui_layer::groupBegin(
            const std::string& title,
            nuklear::panel_flags flags) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_group_begin(&pRes->_context, title.c_str(), static_cast<nk_flags> (flags));
        }        

        void gui_layer::groupEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_group_end(&pRes->_context);
        }

        template<>
        void gui_layer::treePush<nuklear::tree_type::NODE>(
            const std::string& title,
            nuklear::collapse_states state) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_tree_push(&pRes->_context, NK_TREE_NODE, title.c_str(), static_cast<nk_collapse_states> (state));
        }

        template<>
        void gui_layer::treePush<nuklear::tree_type::TAB>(
            const std::string& title,
            nuklear::collapse_states state) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_tree_push(&pRes->_context, NK_TREE_TAB, title.c_str(), static_cast<nk_collapse_states> (state));
        }

        void gui_layer::treePop() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_tree_pop(&pRes->_context);
        }

        bounds<float, float> gui_layer::widgetBounds() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto out = nk_widget_bounds(&pRes->_context);

            return {out.x, out.y, out.w, out.h};
        }

        point<float> gui_layer::widgetPosition() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto out = nk_widget_position(&pRes->_context);

            return {out.x, out.y};
        }

        void gui_layer::text(const char * txt, int len, nuklear::text_alignment alignment) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_text(&pRes->_context, txt, len, static_cast<nk_flags> (alignment));
        }

        void gui_layer::text(const char * txt, int len, nuklear::text_alignment align, const color& c) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_text_colored(&pRes->_context, txt, len, static_cast<nk_flags> (align), nk_rgba(c.r, c.g, c.b, c.a));
        }

        void gui_layer::textWrap(const char * txt, int len) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_text_wrap(&pRes->_context, txt, len);
        }

        void gui_layer::textWrap(const char * txt, int len, const color& c) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_text_wrap_colored(&pRes->_context, txt, len, nk_rgba(c.r, c.g, c.b, c.a));
        }

        void gui_layer::label(const std::string& label, nuklear::text_alignment align) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_label(&pRes->_context, label.c_str(), static_cast<nk_flags> (align));
        }

        void gui_layer::label(const std::string& label, nuklear::text_alignment align, const color& c) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_label_colored(&pRes->_context, label.c_str(), static_cast<nk_flags> (align), nk_rgba(c.r, c.g, c.b, c.a));
        }

        void gui_layer::labelWrap(const std::string& label) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_label_wrap(&pRes->_context, label.c_str());
        }

        void gui_layer::labelWrap(const std::string& label, const color& c) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_label_colored_wrap(&pRes->_context, label.c_str(), nk_rgba(c.r, c.g, c.b, c.a));
        }      

        bool gui_layer::buttonText(const char * title, int len) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_button_text(&pRes->_context, title, len);
        }

        bool gui_layer::buttonLabel(const std::string& title) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_button_label(&pRes->_context, title.c_str());
        }
        
        bool gui_layer::checkLabel(const std::string& txt, bool active) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_check_label(&pRes->_context, txt.c_str(), active);
        }

        bool gui_layer::checkText(const char * txt, int len, bool active) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_check_text(&pRes->_context, txt, len, active);
        }

        bool gui_layer::checkboxLabel(const std::string& label, bool * pActive) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            int active = *pActive;
            int res = nk_checkbox_label(&pRes->_context, label.c_str(), &active);

            *pActive = active;
            
            return res;
        }

        bool gui_layer::checkboxText(const char * txt, int len, bool * pActive) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            int active = *pActive;
            int res = nk_checkbox_text(&pRes->_context, txt, len, &active);

            *pActive = active;

            return res;
        }

        bool gui_layer::radioLabel(const std::string& label, bool * pActive) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            int active = *pActive;
            int res = nk_radio_label(&pRes->_context, label.c_str(), &active);

            *pActive = active;

            return res;
        }

        bool gui_layer::radioText(const char * txt, int len, bool * pActive) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            int active = *pActive;
            int res = nk_radio_text(&pRes->_context, txt, len, &active);

            *pActive = active;

            return res;
        }        

        bool gui_layer::optionLabel(const std::string& label, bool enabled) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_option_label(&pRes->_context, label.c_str(), enabled);
        }

        bool gui_layer::optionText(const char * txt, int len, bool enabled) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_option_text(&pRes->_context, txt, len, enabled);
        }

        bool gui_layer::selectableLabel(const std::string& label, nuklear::text_alignment align, bool * pSelect) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            int select = *pSelect;
            int res = nk_selectable_label(&pRes->_context, label.c_str(), static_cast<nk_flags> (align), &select);

            *pSelect = select;

            return res;
        }

        bool gui_layer::selectLabel(const std::string& label, nuklear::text_alignment align, bool select) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_select_label(&pRes->_context, label.c_str(), static_cast<nk_flags> (align), select);            
        }

        bool gui_layer::selectableText(const char * txt, int len, nuklear::text_alignment align, bool * pSelect) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            int select = *pSelect;
            int res = nk_selectable_text(&pRes->_context, txt, len, static_cast<nk_flags> (align), &select);

            *pSelect = select;

            return res;            
        }

        bool gui_layer::selectText(const char * txt, int len, nuklear::text_alignment align, bool select) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_select_text(&pRes->_context, txt, len, static_cast<nk_flags> (align), select);            
        }

        template<>
        void gui_layer::slider<int>(const std::string& label, int min, int * pValue, int max, int step) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_slider_int(&pRes->_context, min, pValue, max, step);            
        }

        template<>
        void gui_layer::slider<float>(const std::string& label, float min, float * pValue, float max, float step) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_slider_float(&pRes->_context, min, pValue, max, step);            
        }

        template<>
        void gui_layer::progress<nuklear::modify::FIXED>(unsigned int * pCurrent, unsigned int max) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto cur = static_cast<nk_size> (*pCurrent);

            nk_progress(&pRes->_context, &cur, static_cast<nk_size> (max), 0);  

            *pCurrent = static_cast<unsigned int> (cur);
        }

        template<>
        void gui_layer::progress<nuklear::modify::MODIFIABLE>(unsigned int * pCurrent, unsigned int max) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto cur = static_cast<nk_size> (*pCurrent);

            nk_progress(&pRes->_context, &cur, static_cast<nk_size> (max), 1);

            *pCurrent = static_cast<unsigned int> (cur);
        }

        template<>
        color gui_layer::colorPicker<nuklear::color_format::RGB>(const color& c) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto res = nk_color_picker(&pRes->_context, nk_rgb(c.r, c.g, c.b), NK_RGB);

            return color::rgb(res.r, res.g, res.b);
        }

        template<>
        color gui_layer::colorPicker<nuklear::color_format::RGBA>(const color& c) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto res = nk_color_picker(&pRes->_context, nk_rgba(c.r, c.g, c.b, c.a), NK_RGBA);

            return color::rgba(res.r, res.g, res.b, res.a);            
        }        

        template<>
        void gui_layer::property<int>(
            const std::string& label, 
            int min, int * pValue, int max, int step, 
            float incPerPixel) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * >(_pResources.get());

            nk_property_int(&pRes->_context, label.c_str(), min, pValue, max, step, incPerPixel);
        }

        template<>
        void gui_layer::property<float> (
            const std::string& label,
            float min, float * pValue, float max, float step,
            float incPerPixel) noexcept  {

            auto pRes = dynamic_cast<gui_layer_resources * >(_pResources.get());

            nk_property_float(&pRes->_context, label.c_str(), min, pValue, max, step, incPerPixel);
        }
        
        template<>
        void gui_layer::editString<nuklear::plugin_filter::ASCII>(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pCtx = &pRes->_context;
            auto type = static_cast<unsigned int> (flags);

            
            nk_edit_string(pCtx, type, buffer, len, max, nk_filter_ascii);            
        }

        template<>
        void gui_layer::editString<nuklear::plugin_filter::BINARY>(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pCtx = &pRes->_context;
            auto type = static_cast<unsigned int> (flags);

            
            nk_edit_string(pCtx, type, buffer, len, max, nk_filter_binary);            
        }

        template<>
        void gui_layer::editString<nuklear::plugin_filter::DECIMAL>(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pCtx = &pRes->_context;
            auto type = static_cast<unsigned int> (flags);

            
            nk_edit_string(pCtx, type, buffer, len, max, nk_filter_decimal);            
        }

        template<>
        void gui_layer::editString<nuklear::plugin_filter::DEFAULT>(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pCtx = &pRes->_context;
            auto type = static_cast<unsigned int> (flags);

            
            nk_edit_string(pCtx, type, buffer, len, max, nk_filter_default);            
        }

        template<>
        void gui_layer::editString<nuklear::plugin_filter::FLOAT>(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pCtx = &pRes->_context;
            auto type = static_cast<unsigned int> (flags);

            
            nk_edit_string(pCtx, type, buffer, len, max, nk_filter_float);            
        }

        template<>
        void gui_layer::editString<nuklear::plugin_filter::HEX>(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pCtx = &pRes->_context;
            auto type = static_cast<unsigned int> (flags);

            
            nk_edit_string(pCtx, type, buffer, len, max, nk_filter_hex); 
        }

        template<>
        void gui_layer::editString<nuklear::plugin_filter::OCT>(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto pCtx = &pRes->_context;
            auto type = static_cast<unsigned int> (flags);

            
            nk_edit_string(pCtx, type, buffer, len, max, nk_filter_oct);
        }

        template<>
        bool gui_layer::popupBegin<nuklear::popup_type::DYNAMIC>(
            const std::string& title,
            nuklear::panel_flags flags,
            const bounds<float, float>& bounds) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_popup_begin(&pRes->_context, NK_POPUP_DYNAMIC, title.c_str(), static_cast<nk_flags> (flags), nk_rect(bounds.x, bounds.y, bounds.width, bounds.height));
        }

        template<>
        bool gui_layer::popupBegin<nuklear::popup_type::STATIC>(
            const std::string& title,
            nuklear::panel_flags flags,
            const bounds<float, float>& bounds) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_popup_begin(&pRes->_context, NK_POPUP_STATIC, title.c_str(), static_cast<nk_flags> (flags), nk_rect(bounds.x, bounds.y, bounds.width, bounds.height)); 
        }

        void gui_layer::popupEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_popup_end(&pRes->_context);            
        }

        void gui_layer::popupClose() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_popup_close(&pRes->_context);            
        }

        int gui_layer::combo(
            const char ** pItems, int count,
            int selected, int itemHeight,
            const point<float>& size) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_combo(&pRes->_context, pItems, count, selected, itemHeight, nk_vec2(size.x, size.y));
        }

        bool gui_layer::comboBeginText(const char * txt, int len, const point<float>& size) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_combo_begin_text(&pRes->_context, txt, len, nk_vec2(size.x, size.y));            
        }

        bool gui_layer::comboBeginLabel(const std::string& txt, const point<float>& size) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_combo_begin_label(&pRes->_context, txt.c_str(), nk_vec2(size.x, size.y));
        }

        bool gui_layer::comboBeginColor(const color& c, const point<float>& size) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_combo_begin_color(&pRes->_context, nk_rgba(c.r, c.g, c.b, c.a), nk_vec2(size.x, size.y));            
        }

        void gui_layer::comboClose() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_combo_close(&pRes->_context);            
        }

        void gui_layer::comboEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_combo_end(&pRes->_context);            
        }

        bool gui_layer::contextualBegin(const point<float>& pos, const bounds<float, float>& triggerBounds) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_contextual_begin(&pRes->_context, 0, nk_vec2(pos.x, pos.y), nk_rect(triggerBounds.x, triggerBounds.y, triggerBounds.width, triggerBounds.height));            
        }

        bool gui_layer::contextualItemLabel(const std::string& txt, nuklear::text_alignment align) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_contextual_item_label(&pRes->_context, txt.c_str(), static_cast<nk_flags> (align));            
        }

        bool gui_layer::contextualItemText(const char * txt, int len, nuklear::text_alignment align) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_contextual_item_text(&pRes->_context, txt, len, static_cast<nk_flags> (align));            
        }

        void gui_layer::contextualClose() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_contextual_close(&pRes->_context);            
        }

        void gui_layer::contextualEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_contextual_end(&pRes->_context);            
        }

        void gui_layer::tooltip(const std::string& tip) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_tooltip(&pRes->_context, tip.c_str());            
        }

        bool gui_layer::tooltipBegin(float width) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_tooltip_begin(&pRes->_context, width);            
        }

        void gui_layer::tooltipEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_tooltip_end(&pRes->_context);            
        }

        void gui_layer::menubarBegin() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_menubar_begin(&pRes->_context);            
        }

        void gui_layer::menubarEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_menubar_end(&pRes->_context);            
        }

        bool gui_layer::menuBeginLabel(const std::string& label, nuklear::text_alignment align, const point<float>& size) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_menu_begin_label(&pRes->_context, label.c_str(), static_cast<nk_flags> (align), nk_vec2(size.x, size.y));            
        }

        bool gui_layer::menuBeginText(const char * txt, int len, nuklear::text_alignment align, const point<float>& size) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_menu_begin_text(&pRes->_context, txt, len, static_cast<nk_flags> (align), nk_vec2(size.x, size.y));            
        }

        bool gui_layer::menuItemLabel(const std::string& label, nuklear::text_alignment align) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_menu_item_label(&pRes->_context, label.c_str(), static_cast<nk_flags> (align));            
        }

        bool gui_layer::menuItemText(const char * txt, int len, nuklear::text_alignment align) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_menu_item_text(&pRes->_context, txt, len, static_cast<nk_flags> (align));            
        }

        void gui_layer::menuClose() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_menu_close(&pRes->_context);            
        }

        void gui_layer::menuEnd() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_menu_end(&pRes->_context);            
        }

        void gui_layer::styleDefault() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_style_default(&pRes->_context);            
        }

        void gui_layer::styleSetFont(int fontID) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto font = pRes->_fonts[fontID];

            nk_style_set_font(&pRes->_context, &font->handle);    
        }

        void gui_layer::stylePushFont(int fontID) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto font = pRes->_fonts[fontID];

            nk_style_push_font(&pRes->_context, &font->handle); 
        }

        void gui_layer::stylePushFloat(float * pF, float f) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_style_push_float(&pRes->_context, pF, f);            
        }

        void gui_layer::stylePushVec2(point<float> * pV, const point<float>& v) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());
            auto tmpV = nk_vec2(pV->x, pV->y);

            nk_style_push_vec2(&pRes->_context, &tmpV, nk_vec2(v.x, v.y));
            pV->x = tmpV.x;
            pV->y = tmpV.y;
        }

        void gui_layer::stylePopVec2() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_style_pop_vec2(&pRes->_context);            
        }

        void gui_layer::stylePopFloat() noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            nk_style_pop_float(&pRes->_context);            
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::cerr << "[render_engine] nuklear error: " << msg << std::endl;
                __builtin_trap();
            }

            bool gui_layer_resources::handleScrollEvent(double sx, double sy) noexcept {
                _input.scroll.x = sx;
                _input.scroll.y = sy;

                return true;
            }

            bool gui_layer_resources::handleCharEvent(unsigned int codepoint) noexcept {
                if (_device.textLen < MAX_TEXT_SIZE) {
                    _device.text[_device.textLen++] = codepoint;
                }

                return true;
            }

            bool gui_layer_resources::handleMouseButtonEvent(int button, int action, int mods) noexcept {
                if (button != GLFW_MOUSE_BUTTON_LEFT) {
                    return false;
                }

                double x, y;

                auto pWin = reinterpret_cast<GLFWwindow * > (engine::application::getContext());

                glfwGetCursorPos(pWin, &x, &y);

                if (action == GLFW_PRESS) {
                    auto now = glfwGetTime();
                    auto dt = now - _input.lastButtonClick;

                    if (dt > DOUBLE_CLICK_LO && dt < DOUBLE_CLICK_HI) {
                        _input.doubleClick.x = static_cast<float> (x);
                        _input.doubleClick.y = static_cast<float> (y);
                        _input.doubleClick.isDown = true;
                    }

                    _input.lastButtonClick = true;
                } else {
                    _input.doubleClick.isDown = false;
                }

                return true;
            }

            void gui_layer_resources::handleClipboardCopy(nk_handle usr, const char * text, int len) noexcept {
                if (len > 0) {
                    engine::application::setClipboardString(std::string(text, len));
                }
            }

            void gui_layer_resources::handleClipboardPaste(nk_handle usr, nk_text_edit * edit) noexcept {
                auto text = engine::application::getClipboardString();

                if (!text.empty()) {
                    nk_textedit_paste(edit, text.c_str(), text.length());
                }
            }

            void gui_layer_resources::bind() noexcept {
                engine::application::registerCharCallback(
                    std::bind(&gui_layer_resources::handleCharEvent, this,
                        std::placeholders::_1));

                engine::application::registerMouseButtonCallback(
                    std::bind(&gui_layer_resources::handleMouseButtonEvent, this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        std::placeholders::_3));

                engine::application::registerScrollCallback(
                    std::bind(&gui_layer_resources::handleScrollEvent, this, 
                        std::placeholders::_1, 
                        std::placeholders::_2));

                {
                    std::function<void(nk_handle, const char *, int)> fnClipboardCopy = std::bind(
                        &gui_layer_resources::handleClipboardCopy, this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        std::placeholders::_3);

                    _context.clip.copy = fnClipboardCopy.target<void(nk_handle, const char *, int)>();
                }

                {
                    std::function<void(nk_handle, nk_text_edit *)> fnClipboardPaste = std::bind(
                        &gui_layer_resources::handleClipboardPaste, this,
                        std::placeholders::_1,
                        std::placeholders::_2);

                    _context.clip.paste = fnClipboardPaste.target<void(nk_handle, nk_text_edit *)>();
                }
            }

            gui_layer_resources::~gui_layer_resources() {
                nk_font_atlas_clear(&_device.atlas);
                nk_free(&_context);
                nk_buffer_free(&_device.cmds);
            }

            gui_layer_resources::gui_layer_resources(
                const context& ctx,
                const gui_layer_info& info) noexcept {
                    
                _info = info;

                _context.clip.userdata = nk_handle_ptr(nullptr);
                _input.lastButtonClick = 0.0;
                _input.doubleClick = {
                    .x = 0.0F, 
                    .y = 0.0F, 
                    .isDown = false};

                {
                    auto newTexture = graphics::texture({
                        {4, 4, 1},
                        1, 1,
                        {
                            {graphics::mag_filter::NEAREST, graphics::min_filter::NEAREST},
                            {graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::REPEAT},
                            {-1000.0F, 1000.0F}
                        },
                        graphics::internal_format::RGBA8
                    });

                    auto tmp = std::make_unique<unsigned char[]> (4 * 4 * 4);

                    for (int i = 0; i < 4 * 4 * 4; i++) {
                        tmp[i] = static_cast<unsigned char> (0xFF);
                    }

                    newTexture.subImage(0, 0, 0, 0, 4, 4, 1, {
                        graphics::pixel_type::UNSIGNED_BYTE,
                        graphics::pixel_format::RGBA,
                        tmp.get()});

                    std::swap(_device.gl.nullTexture, newTexture);

                    _device.null.texture = nk_handle_id(_device.gl.nullTexture);
                }
                
                nk_font_atlas_init_default(&_device.atlas);
                nk_font_atlas_begin(&_device.atlas);

                for (decltype(info.nFonts) i = 0; i < info.nFonts; i++) {
                    auto& file = info.pFonts[i].file;
                    auto height = info.pFonts[i].height;
                    
                    _fonts.push_back(nk_font_atlas_add_from_file(&_device.atlas, file.c_str(), height, nullptr));
                }

                {
                    //TODO: switch to NK_FONT_ATLAS_ALPHA8

                    int imgWidth = 0;
                    int imgHeight = 0;
                    auto img = nk_font_atlas_bake(&_device.atlas, &imgWidth, &imgHeight, NK_FONT_ATLAS_RGBA32);
                    auto newTexture = graphics::texture({
                        {static_cast<std::size_t> (imgWidth), static_cast<std::size_t> (imgHeight), 1},
                        1, 1,
                        {
                            {graphics::mag_filter::NEAREST, graphics::min_filter::NEAREST},
                            {graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::CLAMP_TO_EDGE, graphics::address_mode::REPEAT},
                            {-1000.0F, 1000.0F}
                        },
                        graphics::internal_format::RGBA8
                    });

                    std::cout << "Width: " << imgWidth << " Height: " << imgHeight << std::endl;

                    newTexture.subImage(0, 0, 0, 0, imgWidth, imgHeight, 1, {
                        graphics::pixel_type::UNSIGNED_BYTE,
                        graphics::pixel_format::RGBA,
                        const_cast<void *> (img)});

                    std::swap(_device.gl.fontTexture, newTexture);
                }
                
                nk_font_atlas_end(&_device.atlas, nk_handle_id(_device.gl.fontTexture), &_device.null);

                if (info.nFonts) {
                    nk_init_default(&_context, &_fonts[info.defaultFontID]->handle);
                } else {
                    nk_init_default(&_context, nullptr);
                }

                if (_device.atlas.default_font) {
                    nk_style_set_font(&_context, &_device.atlas.default_font->handle);
                }            

                _device.textLen = 0;

                nk_buffer_init_default(&_device.cmds);

                
                {
                    auto newVbo = graphics::buffer({
                        graphics::buffer_target::ARRAY, graphics::buffer_usage::STREAM_DRAW,
                        {nullptr, MAX_VERTEX_BUFFER_SIZE}});

                    std::swap(_device.gl.vbo, newVbo);                    
                }

                {
                    auto newEbo = graphics::buffer({
                        graphics::buffer_target::ELEMENT, graphics::buffer_usage::STREAM_DRAW,
                        {nullptr, MAX_ELEMENT_BUFFER_SIZE}});

                    std::swap(_device.gl.ebo, newEbo);
                }                    

                {
                    auto attributes = std::vector<graphics::vertex_attribute_description> ();

                    attributes.push_back({0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0});
                    attributes.push_back({1, graphics::vertex_format::X32Y32_SFLOAT, 8, 0});
                    attributes.push_back({2, graphics::vertex_format::X8Y8Z8W8_UNORM, 16, 0});

                    auto bindings = std::vector<graphics::vertex_binding_description> ();

                    bindings.push_back({0, 20, graphics::vertex_input_rate::PER_VERTEX, &_device.gl.vbo, 0});

                    auto newVao = graphics::vertex_array({
                        attributes.data(), attributes.size(),
                        bindings.data(), bindings.size(),
                        &_device.gl.ebo});

                    std::swap(_device.gl.vao, newVao);
                }
                

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    auto pShaders = std::vector<graphics::shader * > ();

                    pShaders.push_back(&vsh);
                    pShaders.push_back(&fsh);

                    auto attributes = std::vector<graphics::attribute_state_info>();

                    attributes.push_back({"vPosition", 0});
                    attributes.push_back({"vTexCoord", 1});
                    attributes.push_back({"vColor", 2});

                    auto newProgram = graphics::program({
                        pShaders.data(), pShaders.size(),
                        attributes.data(), attributes.size()});

                    if ((_uTexture = newProgram.getUniformLocation("uTexture")) < 0) {
                        _onError("Unable to find uniform \"uTexture\"!");
                    }

                    if ((_uProjection = newProgram.getUniformLocation("uProjection")) < 0) {
                        _onError("Unabel to find uniform \"uProjection\"!");
                    }

                    std::swap(_program, newProgram);
                }
            }
        }
    }
}