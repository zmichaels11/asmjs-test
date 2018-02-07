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
            constexpr std::size_t MAX_TEXT = 1024;
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
                    } gl;
                    
                    unsigned int text[MAX_TEXT];
                    std::size_t textLen;
                } _device;                

                gui_layer_resources(
                    const context& ctx,
                    const gui_layer_info& info) noexcept;

                virtual ~gui_layer_resources();
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

            _pResources = std::make_unique<gui_layer_resources> (ctx, info);
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

        }

        void gui_layer::render() const noexcept {
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
                static auto MAP_ACCESS = graphics::buffer_access::WRITE | graphics::buffer_access::INVALIDATE_BUFFER;

                auto vertices = reinterpret_cast<char * > (pRes->_device.gl.vbo.map(0, MAX_VERTEX_BUFFER_SIZE, MAP_ACCESS));
                auto elements = reinterpret_cast<char * > (pRes->_device.gl.ebo.map(0, MAX_ELEMENT_BUFFER_SIZE, MAP_ACCESS));

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

                    nk_buffer_init_fixed(&vbuf, vertices, MAX_VERTEX_BUFFER_SIZE);
                    nk_buffer_init_fixed(&ebuf, elements, MAX_ELEMENT_BUFFER_SIZE);

                    nk_convert(&pRes->_context, &pRes->_device.cmds, &vbuf, &ebuf, &config);
                }

                pRes->_device.gl.vbo.unmap();
                pRes->_device.gl.ebo.unmap();            
            }

            pRes->_device.gl.vao.bind();

            const nk_draw_command * cmd;
            nk_draw_index * offset = nullptr;

            nk_draw_foreach(cmd, &pRes->_context, &pRes->_device.cmds) {
                if (!cmd->elem_count) {
                    continue;
                }

                auto texture = graphics::texture(cmd->texture.id, graphics::texture_target::TEXTURE_2D);

                texture.bind(0);

                auto sx = static_cast<int>(cmd->clip_rect.x * pRes->_size.scaleW);
                auto sy = static_cast<int>((pRes->_size.height - static_cast<int>(cmd->clip_rect.y + cmd->clip_rect.h)) * pRes->_size.scaleH);
                auto sw = static_cast<int>(cmd->clip_rect.w * pRes->_size.scaleW);
                auto sh = static_cast<int>(cmd->clip_rect.h * pRes->_size.scaleH);

                auto scissorState = graphics::scissor_state_info{true, sx, sy, sw, sh};
                
                graphics::apply(scissorState);
                graphics::draw::elements(graphics::draw_mode::TRIANGLES, cmd->elem_count, graphics::index_type::UNSIGNED_SHORT, offset);

                offset += cmd->elem_count;
            }

            nk_clear(&pRes->_context);

            graphics::apply(graphics::scissor_state_info::defaults());
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

        bool gui_layer::buttonLabel(const std::string& title) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_button_label(&pRes->_context, title.c_str());
        }

        bool gui_layer::buttonText(const char * title, int len) noexcept {
            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            return nk_button_text(&pRes->_context, title, len);
        }

        void gui_layer::editString(
            nuklear::edit_flags flags,
            char * buffer, int * len, int max,
            nuklear::plugin_filter filter) noexcept {

            auto pRes = dynamic_cast<gui_layer_resources * > (_pResources.get());

            
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::cerr << "[render_engine] nuklear error: " << msg << std::endl;
                __builtin_trap();
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

                //TODO: register inputs
                //TODO: register clipboard

                _context.clip.userdata = nk_handle_ptr(nullptr);
                _input.lastButtonClick = 0.0;
                _input.doubleClick = {
                    .x = 0.0F, 
                    .y = 0.0F, 
                    .isDown = false};
                
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

                    newTexture.subImage(0, 0, 0, 0, imgWidth, imgHeight, 1, {
                        graphics::pixel_type::UNSIGNED_BYTE,
                        graphics::pixel_format::RGBA,
                        const_cast<void *> (img)});

                    std::swap(_device.gl.fontTexture, newTexture);
                }
                
                nk_font_atlas_end(&_device.atlas, nk_handle_id(_device.gl.fontTexture), &_device.null);

                nk_init_default(&_context, &_fonts[info.defaultFontID]->handle);

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

                    auto ebo = graphics::buffer({
                        graphics::buffer_target::ARRAY, graphics::buffer_usage::STREAM_DRAW,
                        {nullptr, MAX_ELEMENT_BUFFER_SIZE}});

                    auto attributes = std::vector<graphics::vertex_attribute_description> ();

                    attributes.push_back({0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0});
                    attributes.push_back({1, graphics::vertex_format::X32Y32_SFLOAT, 8, 0});
                    attributes.push_back({2, graphics::vertex_format::X8Y8Z8W8_UNORM, 16, 0});

                    auto bindings = std::vector<graphics::vertex_binding_description> ();

                    bindings.push_back({0, 20, graphics::vertex_input_rate::PER_VERTEX, &_device.gl.vbo, 0});

                    auto newVao = graphics::vertex_array({
                        attributes.data(), attributes.size(),
                        bindings.data(), bindings.size()
                        &_device.gl.ebo});                        
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