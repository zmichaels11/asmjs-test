#include "engine/layers/gui_layer.hpp"

#include <cstddef>

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

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"

#include <string>
#include <vector>

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"

#include "engine/application.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/gui_layer_info.hpp"

namespace engine {
    namespace layers {
        namespace {
            constexpr std::size_t MAX_TEXT = 1024;

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

                gui_layer_resources(const gui_layer_info& info) noexcept;

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

        gui_layer::gui_layer(const gui_layer_info& info) noexcept {
            _pResources = std::make_unique<gui_layer_resources> (info);
        }

        namespace {
            gui_layer_resources::~gui_layer_resources() {
                nk_font_atlas_clear(&_device.atlas);
                nk_free(&_context);
                nk_buffer_free(&_device.cmds);
            }

            gui_layer_resources::gui_layer_resources(const gui_layer_info& info) noexcept {
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

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    auto pShaders = std::vector<graphics::shader * > ();

                    pShaders.push_back(&vsh);
                    pShaders.push_back(&fsh);

                    auto attributes = std::vector<graphics::attribute_state_info>();

                    attributes.push_back({
                        .name = "vPosition", 
                        .location = 0});

                    attributes.push_back({
                        .name = "vTexCoord", 
                        .location = 1});

                    attributes.push_back({
                        .name = "vColor", 
                        .location = 2});

                    auto newProgram = graphics::program({
                        .ppShaders = pShaders.data(), 
                        .shaderCount = pShaders.size(),
                        .pAttributeStates = attributes.data(), 
                        .attributeCount = attributes.size()});

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