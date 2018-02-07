#include "pch.h"
#include "engine/layers/tiled_image.hpp"

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/tile_slot.hpp"
#include "engine/layers/tiled_image_info.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            graphics::address_mode _addressMode(image_scroll_type scroll) {
                switch (scroll) {
                    case image_scroll_type::STATIC:
                        return graphics::address_mode::CLAMP_TO_EDGE;
                    case image_scroll_type::REPEAT:
                        return graphics::address_mode::REPEAT;
                    default:
                        _onError("Unsupported scroll type!");
                        break;
                }
            }

            graphics::mag_filter _magFilter(image_filter_type filter) noexcept {
                switch (filter) {
                    case image_filter_type::NEAREST:
                        return graphics::mag_filter::NEAREST;
                    default:
                        return graphics::mag_filter::LINEAR;
                }
            }

            graphics::min_filter _minFilter(image_filter_type filter) noexcept {
                switch (filter) {
                    case image_filter_type::NEAREST:
                        return graphics::min_filter::NEAREST;
                    case image_filter_type::BILINEAR:
                        return graphics::min_filter::LINEAR;
                    case image_filter_type::TRILINEAR:
                        //TODO: warn
                        return graphics::min_filter::LINEAR;
                    default:
                        _onError("Unsupported filter type!");
                        break;
                }
            }            

            struct vec2_t {
                float x, y;
            };

            struct tiled_image_resources : public base_resources {
                const context * _pCtx;
                tiled_image_info _info;
                bool _dirty;
                bool _redraw;
                float _scaleW;
                float _scaleH;

#if defined(__EMSCRIPTEN__)
                std::unique_ptr<tile_slot[]> _tileSlots;
#endif

                tile_slot * _pTileSlotAccessor;
                std::size_t _tileCount;                

                struct vbo_t {
                    graphics::buffer select;
                    graphics::buffer position;
                    graphics::buffer imageView;
                } _vbos;

                graphics::clear_state_info _clear;
                graphics::scissor_state_info _scissor;
                graphics::viewport_state_info _viewport;
                graphics::framebuffer _fb;
                graphics::texture _texture;                
                graphics::vertex_array _vao;                

                tiled_image_resources(const tiled_image_info& info) noexcept;

                virtual ~tiled_image_resources() {}
            };

            graphics::program _program;
            int _uTileSize;
            int _uImage;

            const std::string BASE_SHADER_PATH = "data/shaders/tiled_image_renderer/";
#if defined (GL)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "330_core.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "330_core.frag";
#elif defined (GLES30)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "300_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "300_es.frag";
#elif defined (GLES20)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "100_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "100_es.frag";
#else
            const std::string VERTEX_SHADER_PATH = "";
            const std::string FRAGMENT_SHADER_PATH = "";
#error "No GL defined!"
#endif
        }

        tiled_image::tiled_image(const tiled_image_info& info) noexcept {

            _pResources = std::make_unique<tiled_image_resources> (info);
        }

        void tiled_image::beginWrite() noexcept {
            constexpr static auto MAP_ACCESS = graphics::buffer_access::WRITE | graphics::buffer_access::INVALIDATE_BUFFER;
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get()); 

#if defined(__EMSCRIPTEN__)
            res->_pTileSlotAccessor = res->_tileSlots.get();
#else
            auto mappedSize = res->_tileCount * sizeof(tile_slot);
            auto mappedData = res->_vbos.imageView.map(0, mappedSize, MAP_ACCESS);            

            res->_pTileSlotAccessor = reinterpret_cast<tile_slot * > (mappedData);
#endif

            res->_redraw = false;
        }

        void tiled_image::endWrite() noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

#if defined(__EMSCRIPTEN__)
            auto tileDataSize = res->_tileCount * sizeof(tile_slot);

            res->_vbos.imageView.invalidate();
            res->_vbos.imageView.subData(0, res->_tileSlots.get(), tileDataSize);
#else
            res->_vbos.imageView.unmap();
#endif

            res->_pTileSlotAccessor = nullptr;

            if (res->_dirty) {
                res->_dirty = false;
                res->_redraw = true;
            }            
        }

        void tiled_image::invalidate() noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());
            
            res->_dirty = true;
        }

        void tiled_image::render() const noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

            if (!res->_redraw) {
                return;
            }

            auto pTileSheet = res->_pCtx->getSpriteSheet(res->_info.tileSheetID);
            auto pTexture = reinterpret_cast<const graphics::texture * > (pTileSheet->getTexture());            

            pTexture->bind(0);

            _program.use();

            graphics::uniform::setUniform1(_uImage, 0);
            graphics::uniform::setUniform2(_uTileSize, res->_scaleW, res->_scaleH);

            res->_fb.bind();
            
            auto color0 = graphics::draw_buffer::COLOR_ATTACHMENT0;

            graphics::framebuffer::drawBuffers(&color0);
            
            graphics::apply(res->_viewport);
            graphics::apply(res->_scissor);
            graphics::apply(res->_clear);
            graphics::apply(graphics::blend_state_info::premultipliedAlpha());

            res->_vao.bind();

            graphics::draw::arraysInstanced(graphics::draw_mode::TRIANGLE_STRIP, 0, 4, res->_tileCount);
        }

        const void * tiled_image::getTexture() const noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

            if (static_cast<int> (res->_texture)) {
                return reinterpret_cast<const void * > (&res->_texture);
            } else {
                return nullptr;
            }
        }

        tile_slot ** tiled_image::fetchTileSlots() noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());            

            return &res->_pTileSlotAccessor;
        }

        void tiled_image::bind(const context * pCtx) noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());

            res->_pCtx = pCtx;
        }

        const image_view& tiled_image::getImageView(int id) const noexcept {
            auto res = dynamic_cast<tiled_image_resources * > (_pResources.get());
            auto pTileSheet = res->_pCtx->getSpriteSheet(res->_info.tileSheetID);

            return pTileSheet->getSprite(id);
        }

        const tiled_image_info& tiled_image::getInfo() const noexcept {
            auto res = dynamic_cast<const tiled_image_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::cerr << "[render_engine] tiled_image error: " << msg << std::endl;
                __builtin_trap();
            }

            tiled_image_resources::tiled_image_resources(const tiled_image_info& info) noexcept {
                _pCtx = nullptr;
                _dirty = false;
                _redraw = false;
                _info = info;

                auto textureWidth = info.dim.columns * info.tileSize.width;
                auto textureHeight = info.dim.rows * info.tileSize.width;

                _tileCount = info.dim.columns * info.dim.rows;

#if defined(__EMSCRIPTEN__)
                _tileSlots = std::make_unique<tile_slot[]> (_tileCount);
#endif

                // OpenGL screen space is [-1.0, -1.0] to [1.0, 1.0], so scale appropriately.
                _scaleW = 2.0F / static_cast<float> (info.dim.columns);
                _scaleH = 2.0F / static_cast<float> (info.dim.rows);

                _clear = {graphics::clear_buffer::COLOR, {
                    util::normalize(info.clearColor.r),
                    util::normalize(info.clearColor.g),
                    util::normalize(info.clearColor.b),
                    util::normalize(info.clearColor.a)}};

                _viewport = {0, 0, static_cast<int> (textureWidth), static_cast<int> (textureHeight)};
                _scissor = {true, 0, 0, static_cast<int> (textureWidth), static_cast<int> (textureHeight)};
                
                {
                    auto newTexture = graphics::texture({
                        {textureWidth, textureHeight, 1}, // should this be power-of-2?
                        1, 1,
                        {
                            {_magFilter(info.filter), _minFilter(info.filter)},
                            {_addressMode(info.scroll.horizontal), _addressMode(info.scroll.vertical), graphics::address_mode::REPEAT},
                            {-1000.0, 1000.0}
                        },
                        graphics::internal_format::RGBA8});

                    std::swap(_texture, newTexture);
                }

                {
                    auto attachmentInfo = std::vector<graphics::attachment_info>();        

                    attachmentInfo.push_back(graphics::attachment_info(&_texture));

                    auto newFB = graphics::framebuffer({
                        .pAttachments = attachmentInfo.data(), 
                        .nAttachments = attachmentInfo.size()});

                    std::swap(_fb, newFB);
                }       

                {
                    auto positionData = std::vector<vec2_t>();

                    positionData.reserve(_tileCount);                    

                    for (decltype(info.dim.rows) j = 0; j < info.dim.rows; j++) {
                        auto y = static_cast<float> (j) * _scaleH - 1.0F;

                        for (decltype(info.dim.columns) i = 0; i < info.dim.columns; i++) {
                            auto x = static_cast<float> (i) * _scaleW - 1.0F;

                            positionData.push_back({x, y});
                        }
                    }

                    auto newPosition = graphics::buffer({
                        graphics::buffer_target::ARRAY,
                        graphics::buffer_usage::STATIC_DRAW,
                        {positionData.data(), positionData.size() * sizeof(vec2_t)}});

                    std::swap(_vbos.position, newPosition);
                }

                {
                    //TODO: this should be constant
                    auto selectData = std::vector<vec2_t>();

                    // 4 vertices per primitive
                    selectData.reserve(4);

                    selectData.push_back({0.0F, 0.0F});
                    selectData.push_back({0.0F, 1.0F});
                    selectData.push_back({1.0F, 0.0F});
                    selectData.push_back({1.0F, 1.0F});                    

                    auto newSelect = graphics::buffer({
                        graphics::buffer_target::ARRAY,
                        graphics::buffer_usage::STATIC_DRAW,
                        {selectData.data(), sizeof(vec2_t) * selectData.size()}});

                    std::swap(_vbos.select, newSelect);
                }

                {
                    auto newImageView = graphics::buffer({
                        graphics::buffer_target::ARRAY,
                        graphics::buffer_usage::STREAM_DRAW,
                        {nullptr, sizeof(tile_slot) * _tileCount}});

                    std::swap(_vbos.imageView, newImageView);
                }

                constexpr int A_SELECT = 0;
                constexpr int A_POSITION = 1;
                constexpr int A_FRAME_INDEX = 2;
                constexpr int A_FRAME_VIEW = 3;                

                {
                    constexpr auto vec2_32 = graphics::vertex_format::X32Y32_SFLOAT;
                    constexpr auto vec2_16_unorm = graphics::vertex_format::X16Y16_UNORM;
                    constexpr auto float_32 = graphics::vertex_format::X32_SFLOAT;

                    constexpr int B_SELECT = 0;
                    constexpr int B_POSITION = 1;
                    constexpr int B_FRAME = 2;

                    auto attributes = std::vector<graphics::vertex_attribute_description> ();                    

                    attributes.push_back({
                        location: A_SELECT, 
                        format: vec2_32, 
                        offset: 0, 
                        binding: B_SELECT});

                    attributes.push_back({
                        location: A_POSITION, 
                        format: vec2_32, 
                        offset: 0, 
                        binding: B_POSITION});

                    attributes.push_back({
                        location: A_FRAME_INDEX, 
                        format: float_32, 
                        offset: 0, 
                        binding: B_FRAME});

                    attributes.push_back({
                        location: A_FRAME_VIEW, 
                        format: vec2_16_unorm, 
                        offset: offsetof(image_view, u), 
                        binding: B_FRAME});

                    constexpr int TIGHTLY_PACKED = 0;

                    auto bindings = std::vector<graphics::vertex_binding_description> ();

                    bindings.push_back({
                        binding: B_SELECT, 
                        stride: TIGHTLY_PACKED, 
                        inputRate: graphics::vertex_input_rate:: PER_VERTEX, 
                        pBuffer: &_vbos.select, 
                        offset: 0});

                    bindings.push_back({
                        binding: B_POSITION, 
                        stride: TIGHTLY_PACKED, 
                        inputRate: graphics::vertex_input_rate::PER_INSTANCE, 
                        pBuffer: &_vbos.position, 
                        offset: 0});
                        
                    bindings.push_back({
                        binding: B_FRAME, 
                        stride: sizeof(image_view), 
                        inputRate: graphics::vertex_input_rate::PER_INSTANCE, 
                        pBuffer: &_vbos.imageView, 
                        offset: 0});

                    auto newVao = graphics::vertex_array({
                        pAttributes: attributes.data(), 
                        nAttributes: attributes.size(), 
                        pBindings: bindings.data(), 
                        nBindings: bindings.size()});

                    std::swap(_vao, newVao);
                }

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    auto pShaders = std::vector<graphics::shader * >();

                    pShaders.push_back(&vsh);
                    pShaders.push_back(&fsh);

                    auto attributes = std::vector<graphics::attribute_state_info> ();

                    attributes.push_back({
                        name: "vSelect", 
                        location: A_SELECT});

                    attributes.push_back({
                        name: "vPosition", 
                        location: A_POSITION});

                    attributes.push_back({
                        name: "vFrameIndex", 
                        location: A_FRAME_INDEX});

                    attributes.push_back({
                        name: "vFrameView", 
                        location: A_FRAME_VIEW});                    

                    auto newProgram = graphics::program({
                        ppShaders: pShaders.data(), 
                        nShaders: pShaders.size(), 
                        pAttributes: attributes.data(), 
                        nAttributes: attributes.size()});

                    std::swap(_program, newProgram);

                    if ((_uImage = _program.getUniformLocation("uImage")) < 0) {
                        _onError("Could not find uniform: \"uImage\"!");
                    }

                    if ((_uTileSize = _program.getUniformLocation("uTileSize")) < 0) {
                        _onError("Could not find uniform: \"uTileSize\"!");
                    }
                }
            }
        }
    }
}