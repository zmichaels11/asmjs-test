#include "pch.h"
#include "engine/layers/background_layer.hpp"

#include "engine/layers/background_layer_info.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/image_scroll_type.hpp"

namespace engine {
    namespace layers {
        namespace {
            void _onError(const std::string& msg) noexcept;

            graphics::address_mode _scrollType(image_scroll_type type) noexcept;

            struct background_layer_resources : public base_resources {
                background_layer_info _info;
                const context * _pCtx;    

                float _transform[4];
                float _origin[2];
                float _scroll[2];
				float _projection[16];
                
                graphics::vertex_array _vao;

                background_layer_resources(
                    const context& ctx,
                    const background_layer_info& info) noexcept;

                virtual ~background_layer_resources() {}
            };

            graphics::program _program;
            int _uImage;
            int _uTransform;
            int _uOrigin;
            int _uScroll;
			int _uProjection;

            const std::string BASE_SHADER_PATH = "data/shaders/background_renderer/";
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

        background_layer::background_layer(
            const context& ctx,
            const background_layer_info& info) noexcept {

            _pResources = std::make_unique<background_layer_resources> (ctx, info);
        }

        void background_layer::beginWrite() noexcept {}

        void background_layer::endWrite() noexcept {}

        void background_layer::render() noexcept {
            auto res = dynamic_cast<const background_layer_resources * > (_pResources.get());
            auto * renderable = res->_pCtx->getRenderableImage(res->_info.renderableID);
            auto * pTexture = reinterpret_cast<const graphics::texture * > (renderable->getTexture());                    

            if (pTexture) {
                _program.use();

                graphics::uniform::setUniform1(_uImage, 0);
                graphics::uniform::setUniform2(_uOrigin, 1, res->_origin);
                graphics::uniform::setUniform2(_uScroll, 1, res->_scroll);
                graphics::uniform::setUniformMatrix2(_uTransform, 1, res->_transform);
				graphics::uniform::setUniformMatrix4(_uProjection, 1, res->_projection);
                
                pTexture->bind(0);
                res->_vao.bind();

                graphics::draw::arrays(graphics::draw_mode::TRIANGLE_STRIP, 0, 4);
            }            
        }

        void background_layer::invalidate() noexcept {}

        void background_layer::setProjection(const math::mat4& projection) noexcept {
			auto res = dynamic_cast<background_layer_resources * > (_pResources.get());
			
			projection.data(res->_projection);
		}

        void background_layer::setProjection(const float * projection) noexcept {
			auto res = dynamic_cast<background_layer_resources * > (_pResources.get());
			
			std::memcpy(res->_projection, projection, 16 * sizeof(float));
		}

        void background_layer::scroll(float h, float v) noexcept {
            auto res = dynamic_cast<background_layer_resources * > (_pResources.get());

            res->_scroll[0] = h;
            res->_scroll[1] = v;
        }

        void background_layer::setOrigin(float x, float y) noexcept {
            auto res = dynamic_cast<background_layer_resources * > (_pResources.get());
            
            res->_origin[0] = x;
            res->_origin[1] = y;
        }

        void background_layer::setTransform(const math::mat2& transform) noexcept {
            auto res = dynamic_cast<background_layer_resources * > (_pResources.get());

            transform.data(res->_transform);
        }

        void background_layer::setTransform(const float * transform) noexcept {
            auto res = dynamic_cast<background_layer_resources * > (_pResources.get());

            std::memcpy(res->_transform, transform, 4 * sizeof(float));
        }

        void background_layer::setTransform(float a, float b, float c, float d) noexcept {
            auto res = dynamic_cast<background_layer_resources * > (_pResources.get());

            res->_transform[0] = a;
            res->_transform[1] = b;
            res->_transform[2] = c;
            res->_transform[3] = d;
        }

        const background_layer_info& background_layer::getInfo() const noexcept {
            auto res = dynamic_cast<background_layer_resources * > (_pResources.get());

            return res->_info;
        }

        namespace {
            void _onError(const std::string& msg) noexcept {
                std::cerr << "[render engine] background_layer error: " << msg << std::endl;
                __builtin_trap();
            }

            graphics::address_mode _scrollType(image_scroll_type type) noexcept {
                switch (type) {
                    case image_scroll_type::REPEAT:
                        return graphics::address_mode::REPEAT;
                    case image_scroll_type::STATIC:
                        return graphics::address_mode::CLAMP_TO_EDGE;
                    default:
                        _onError("Unsupported image_scroll_type!");
                }
            }

            background_layer_resources::background_layer_resources(
                const context& ctx,
                const background_layer_info& info) noexcept {

                _info = info;
				
				_projection[0] = 1.0F;
				_projection[1] = 0.0F;
				_projection[2] = 0.0F;
				_projection[3] = 0.0F;
				_projection[4] = 0.0F;
				_projection[5] = 1.0F;
				_projection[6] = 0.0F;
				_projection[7] = 0.0F;
				_projection[8] = 0.0F;
				_projection[9] = 0.0F;
				_projection[10] = 1.0F;
				_projection[11] = 0.0F;
				_projection[12] = 0.0F;
				_projection[13] = 0.0F;
				_projection[14] = 0.0F;
				_projection[15] = 1.0F;				

                _transform[0] = 1.0F;
                _transform[1] = 0.0F;
                _transform[2] = 0.0F;
                _transform[3] = 1.0F;

                _origin[0] = 0.0F;
                _origin[1] = 0.0F;

                _scroll[0] = 0.0F;
                _scroll[1] = 0.0F;

                _pCtx = &ctx;

                auto newVao = graphics::vertex_array::empty();

                std::swap(_vao, newVao);

                if (!_program) {
                    auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                    auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                    auto pShaders = std::vector<graphics::shader * > ();

                    pShaders.push_back(&vsh);
                    pShaders.push_back(&fsh);                    

                    auto newProgram = graphics::program({
                        ppShaders: pShaders.data(), 
                        nShaders: pShaders.size()
                    });

                    std::swap(_program, newProgram);

                    if ((_uImage = _program.getUniformLocation("uImage")) < 0) {
                        _onError("Could not find uniform \"uImage\"!");
                    }

                    if ((_uScroll = _program.getUniformLocation("uScroll")) < 0) {
                        _onError("Could not find uniform \"uScroll\"!");
                    }

                    if ((_uOrigin = _program.getUniformLocation("uOrigin")) < 0) {
                        _onError("Could not find uniform \"uOrigin\"!");
                    }
                    
                    if ((_uTransform = _program.getUniformLocation("uTransform")) < 0) {
                        _onError("Could not find uniform \"uTransform\"!");
                    }

					if ((_uProjection = _program.getUniformLocation("uProjection")) < 0) {
                        _onError("Could not find uniform \"uProjection\"!");
                    }
                }
            }
        }
    }
}