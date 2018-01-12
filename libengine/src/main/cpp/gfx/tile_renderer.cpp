#include "engine/gfx/tile_renderer.hpp"

#include <cstddef>
#include <cstdio>
#include <cstring>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "engine/gfx/base_resources.hpp"

#include "graphics/buffer.hpp"
#include "graphics/operation.hpp"
#include "graphics/program.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"

namespace engine {
    namespace gfx {
        namespace {
            struct tile {
                float u0, v0, u1, v1;
            };

            struct vertex {
                float x0, y0, x1, y1;
            };

            struct tile_renderer_resources : public virtual base_resources {
                tile_renderer_info info;
                graphics::buffer index;
                graphics::buffer vertices;
                graphics::buffer tiles;
                graphics::vertex_array model;
                std::unique_ptr<tile[]> tileData;
                std::size_t tileCount;
                bool isDirty;
                float projection[16];

                tile_renderer_resources(const tile_renderer_info& info) noexcept;

                virtual ~tile_renderer_resources();
            };

            graphics::program program;
            int uTilesheet;
            int uProjection;

            const std::string BASE_SHADER_PATH = "data/shaders/tile_renderer/";
#if defined(GL)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "330_core.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "330_core.frag";
#elif defined(GLES30)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "300_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_PATH + "300_es.frag";
#elif defined(GLES20)
            const std::string VERTEX_SHADER_PATH = BASE_SHADER_PATH + "100_es.vert";
            const std::string FRAGMENT_SHADER_PATH = BASE_SHADER_pATH + "100_es.frag";
#else
            const std::string VERTEX_SHADER_PATH = "";
            const std::string FRAGMENT_SHADER_PATH = "";
#error "No GL defined!"
#endif
        }

        tile_renderer::tile_renderer(const tile_renderer_info& info) noexcept {
            _pResources = std::make_unique<tile_renderer_resources>(info);
        }

        void tile_renderer::reset() noexcept {
            auto res = dynamic_cast<tile_renderer_resources *> (_pResources.get());
            auto tileDataSize = res->tileCount * sizeof(tile);

            std::memset(res->tileData.get(), 0, tileDataSize);
        }

        void tile_renderer::pushData(const void * pData) noexcept {
            auto res = dynamic_cast<tile_renderer_resources * > (_pResources.get());
            auto data = reinterpret_cast<const tile_renderer::data * > (pData);

            switch (data->type) {
                case tile_renderer::data_type::PROJECTION: {
                    std::memcpy(res->projection, data->payload.projection, sizeof(float) * 16);
                    break;
                }
                case tile_renderer::data_type::TILE: {
                    auto idx = res->info.rows * data->payload.tile.y + data->payload.tile.x;

                    //TODO: translate tileId into tile
                    res->tileData[idx] = tile();
                    break;
                }
                default: {
                    std::printf("[renderer] tile_renderer warning: Undefined push data!\n");
                    break;
                }
            }            
        }

        void tile_renderer::update() noexcept {
            auto res = dynamic_cast<tile_renderer_resources * > (_pResources.get());

            if (res->isDirty) {
                res->tiles.invalidate();
                res->tiles.subData(0, res->tileData.get(), sizeof(tile) * res->tileCount);
                res->isDirty = false;
            }
        }

        void tile_renderer::render() const noexcept {
            if (!program) {
                auto vsh = graphics::shader::makeVertex(VERTEX_SHADER_PATH);
                auto fsh = graphics::shader::makeFragment(FRAGMENT_SHADER_PATH);

                graphics::attribute_state_info attribs[] = {
                    {"vIndex", 0},
                    {"vPosition", 1},
                    {"vTexCoord", 2}
                };

                decltype(&vsh) shaders[] = {&vsh, &fsh};
                
                program = graphics::program(graphics::program_info{shaders, 2, attribs, 3});

                uTilesheet = program.getUniformLocation("uTilesheet");
                uProjection = program.getUniformLocation("uProjection");
            }

            auto res = dynamic_cast<tile_renderer_resources * > (_pResources.get());

            program.use();

            graphics::uniform::setUniform1(uTilesheet, 0);
            graphics::uniform::setUniformMatrix4(uProjection, 1, res->projection);

            res->model.bind();
            graphics::draw::arraysInstanced(graphics::draw_mode::TRIANGLE_STRIP, 0, 4, res->tileCount);
        }

        namespace {
            tile_renderer_resources::tile_renderer_resources(const tile_renderer_info& info) noexcept {
                this->info = info;
                this->isDirty = false;

                tileCount = info.rows * info.columns;
                
                auto vertexCount = 4 * tileCount;
                auto vertexData = std::vector<vertex>();

                vertexData.reserve(vertexCount);

                auto tw = static_cast<float> (info.tileWidth);
                auto th = static_cast<float> (info.tileHeight);

                for (decltype(info.columns) y = 0; y < info.columns; y++) {
                    auto y0 = static_cast<float> (y) * th;
                    auto y1 = y0 + th;

                    for (decltype(info.rows) x = 0; x < info.rows; x++) {
                        auto x0 = static_cast<float> (x) * tw;
                        auto x1 = x0 + tw;

                        auto v0 = vertex{x0, y0};
                        auto v1 = vertex{x0, y1};
                        auto v2 = vertex{x1, y0};
                        auto v3 = vertex{x1, y1};                       

                        vertexData.push_back(v0);
                        vertexData.push_back(v1);                        
                        vertexData.push_back(v2);
                        vertexData.push_back(v3);
                    }
                }

                this->tileData = std::make_unique<tile[]> (tileCount);

                auto vertexDataSize = sizeof(vertex) * vertexCount;
                auto tileDataSize = sizeof(tile) * tileCount;                

                float indexData[] = {
                    0.0F, 0.0F,
                    0.0F, 1.0F,
                    1.0F, 0.0F,
                    1.0F, 1.0F};

                this->index = graphics::buffer(graphics::buffer_info{graphics::buffer_target::ARRAY, graphics::buffer_usage::STATIC_DRAW, {indexData, sizeof(indexData)}});
                this->vertices = graphics::buffer(graphics::buffer_info{graphics::buffer_target::ARRAY, graphics::buffer_usage::STATIC_DRAW, {vertexData.data(), vertexDataSize}});
                this->tiles = graphics::buffer(graphics::buffer_info{graphics::buffer_target::ARRAY, graphics::buffer_usage::DYNAMIC_DRAW, {nullptr, tileDataSize}});                

                auto indexBinding = graphics::vertex_binding_description{0, 0, 0, &index, 0};
                auto vertexBinding = graphics::vertex_binding_description{1, sizeof(vertex), 1, &vertices, 0}; // every quad
                auto tileBinding = graphics::vertex_binding_description{2, sizeof(tile), 1, &tiles, 0}; // every quad

                auto aIndex = graphics::vertex_attribute_description{0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0};
                auto aVertex = graphics::vertex_attribute_description{0, graphics::vertex_format::X32Y32Z32W32_SFLOAT, 0, 1};
                auto aTile = graphics::vertex_attribute_description{1, graphics::vertex_format::X32Y32Z32W32_SFLOAT, 0, 2};

                decltype(&aVertex) attribs[] = {&aIndex, &aVertex, &aTile};
                decltype(&vertexBinding) bindings[] = {&indexBinding, &vertexBinding, &tileBinding};

                this->model = graphics::vertex_array(graphics::vertex_array_info{attribs, 3, bindings, 3, nullptr});
            }

            tile_renderer_resources::~tile_renderer_resources() {}
        }
    }
}