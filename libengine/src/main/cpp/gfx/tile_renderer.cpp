#include "engine/gfx/tile_renderer.hpp"

#include <cstddef>

#include <memory>
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
                graphics::buffer vertices;
                graphics::buffer tiles;
                graphics::vertex_array model;
                std::unique_ptr<tile[]> tileData;
                std::size_t tileCount;
                bool isDirty;

                tile_renderer_resources(const tile_renderer_info& info) noexcept;

                virtual ~tile_renderer_resources();
            };

            graphics::program PROGRAM;
            int uTilesheet;
        }

        tile_renderer::tile_renderer(const tile_renderer_info& info) noexcept {
            _pResources = std::make_unique<tile_renderer_resources>(info);
        }

        void tile_renderer::reset() noexcept {
            auto res = dynamic_cast<tile_renderer_resources *> (_pResources.get());
            auto tileCount = res->info.columns * res->info.rows;

            for (decltype(tileCount) i = 0; i < tileCount; i++) {
                res->tileData[i] = tile();
            }

            res->isDirty = true;
        }

        void tile_renderer::pushData(const void * pData) noexcept {
            auto tileData = reinterpret_cast<const tile_renderer::tile_data * > (pData);
            auto res = dynamic_cast<tile_renderer_resources * > (_pResources.get());
            auto idx = res->info.rows * tileData->y + tileData->x;

            //TODO: translate tileData->tileId into a tile object            
            res->tileData[idx] = tile();
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
            if (!PROGRAM) {
                //TODO: init program
            }

            auto res = dynamic_cast<tile_renderer_resources * > (_pResources.get());

            PROGRAM.use();

            graphics::uniform::setUniform1(uTilesheet, 0);

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

                for (decltype(info.columns) y = 0; y < info.columns; y++) {
                    for (decltype(info.rows) x = 0; x < info.rows; x++) {
                        auto x0 = x * info.tileWidth;
                        auto y0 = y * info.tileHeight;
                        auto x1 = x0 + info.tileWidth;
                        auto y1 = y0 + info.tileHeight;

                        auto v0 = vertex{x0, y0};
                        auto v1 = vertex{x0, y1};
                        auto v2 = vertex{x1, y0};
                        auto v3 = vertex{x1, y1};

                        // v0-v2
                        // | / |
                        // v1-v3

                        vertexData.push_back(v0);
                        vertexData.push_back(v1);                        
                        vertexData.push_back(v2);
                        vertexData.push_back(v3);
                    }
                }

                this->tileData = std::make_unique<tile[]> (tileCount);

                auto vertexDataSize = sizeof(vertex) * vertexCount;
                auto tileDataSize = sizeof(tile) * tileCount;                

                auto newVertices = graphics::buffer(graphics::buffer_info{graphics::buffer_target::ARRAY, graphics::buffer_usage::STATIC_DRAW, {vertexData.data(), vertexDataSize}});
                auto newTiles = graphics::buffer(graphics::buffer_info{graphics::buffer_target::ARRAY, graphics::buffer_usage::DYNAMIC_DRAW, {nullptr, tileDataSize}});

                std::swap(this->vertices, newVertices);
                std::swap(this->tiles, newTiles);

                auto vertexBinding = graphics::vertex_binding_description{0, sizeof(vertex), 1, &vertices, 0}; // every quad
                auto tileBinding = graphics::vertex_binding_description{1, sizeof(tile), 1, &tiles, 0}; // every quad

                auto aVertex = graphics::vertex_attribute_description{0, graphics::vertex_format::X32Y32_SFLOAT, 0, 0};
                auto aTile = graphics::vertex_attribute_description{1, graphics::vertex_format::X32Y32Z32W32_SFLOAT, 0, 1};

                decltype(&aVertex) attribs[] = {&aVertex, &aTile};
                decltype(&vertexBinding) bindings[] = {&vertexBinding, &tileBinding};

                auto newModel = graphics::vertex_array(graphics::vertex_array_info{attribs, 2, bindings, 2, nullptr});

                std::swap(this->model, newModel);
            }

            tile_renderer_resources::~tile_renderer_resources() {}
        }
    }
}