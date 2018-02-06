#include "engine/application.hpp"

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "graphics/image.hpp"

#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"
#include "engine/layers/sprite_sheet_info.hpp"
#include "engine/layers/tiled_image.hpp"

constexpr unsigned int TILES_ACROSS = 32;
constexpr unsigned int TILES_DOWN = 32;
constexpr unsigned int TILE_WIDTH = 32;
constexpr unsigned int TILE_HEIGHT = 32;
constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr float SCROLL_SPEED = 0.001F;
constexpr auto HINTS = engine::application_hint::VSYNC;

auto _loadTile(
    std::vector<graphics::image * >& imgRefs,
    const std::string& res) noexcept -> decltype(graphics::image::read("")) {

    auto ptr = graphics::image::read(res);

    imgRefs.push_back(ptr.get());

    return ptr;    
}

enum class direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct tile_test_data {
    engine::layers::tile_slot ** ppTileSlots;
    engine::layers::image_view tiles[2];
    bool built;

    struct scroll_t {
        direction dir ;
        float x;
        float y;
    } scroll;
};

int main(int argc, char** argv) {
    engine::application::init({"Tiles Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {1, 0}, HINTS});        

    auto tileCache = std::vector<decltype(graphics::image::read(""))> ();
    auto ppTileImages = std::vector<graphics::image * > ();

    tileCache.push_back(_loadTile(ppTileImages, "data/images/tiles/castle0.png"));
    tileCache.push_back(_loadTile(ppTileImages, "data/images/tiles/dirt0.png"));

    auto pSpriteSheetInfos = std::vector<engine::layers::sprite_sheet_info>();

    pSpriteSheetInfos.push_back({
        ppTileImages.data(), ppTileImages.size(),
        engine::layers::sprite_sheet_hint::LAYERED
    });

    auto backgroundInfo = engine::layers::background_layer_info{0};    
    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();
    auto backgroundLayerInfo = engine::layers::scene_layer_info::init(backgroundInfo);

    backgroundLayerInfo.ext = {
        engine::layers::scene_layer_hint::CLEAR,
        {
            engine::layers::clear_type::COLOR,
            engine::layers::color::rgb(120, 80, 20)
        }};

    pLayerInfos.push_back(backgroundLayerInfo);

    auto pRenderableInfos = std::vector<engine::layers::renderable_info>();
    auto tiledBackgroundInfo = engine::layers::renderable_info();

    tiledBackgroundInfo.type = engine::layers::renderable_type::TILED_IMAGE;
    tiledBackgroundInfo.info.tiledImageInfo = {
        .dim = {TILES_ACROSS, TILES_DOWN},
        .tileSize = {TILE_WIDTH, TILE_HEIGHT},
        .scroll = {engine::layers::image_scroll_type::REPEAT, engine::layers::image_scroll_type::REPEAT},
        .filter = engine::layers::image_filter_type::BILINEAR,
        .clearColor = engine::layers::color::rgb(20, 80, 120),
        .tileSheetID = 0
    };

    pRenderableInfos.push_back(tiledBackgroundInfo);

    auto sceneInfo = engine::layers::scene_info{
        .contextInfo = {
            .pSpriteInfos = pSpriteSheetInfos.data(), 
            .nSpriteInfos = pSpriteSheetInfos.size(),
            .pFontInfos = nullptr, 
            .nFontInfos = 0,
            .pRenderableInfos = pRenderableInfos.data(), 
            .nRenderableInfos = pRenderableInfos.size()},            
        .pLayerInfos = pLayerInfos.data(), 
        .nLayerInfos = pLayerInfos.size()};

    std::random_device rd;    
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> tileDist(0, 2);
    std::uniform_real_distribution<double> scrollDist(0.0, 1.0);

    engine::application::setOnUpdate([&](auto pUserData) {        
        auto pScene = engine::application::getScene();
        auto pLayer = dynamic_cast<engine::layers::background_layer * > (pScene->getLayer(0));
        auto pTileTestData = reinterpret_cast<tile_test_data * > (pUserData);
            
        if (scrollDist(mt) > 0.995) {
            auto dirSelect = scrollDist(mt);

            if (dirSelect > 0.75) {
                pTileTestData->scroll.dir = direction::UP;
            } else if (dirSelect > 0.5) {
                pTileTestData->scroll.dir = direction::DOWN;
            } else if (dirSelect > 0.25) {
                pTileTestData->scroll.dir = direction::LEFT;
            } else {
                pTileTestData->scroll.dir = direction::RIGHT;
            }
        }

        switch (pTileTestData->scroll.dir) {
            case direction::UP:
                pTileTestData->scroll.y -= SCROLL_SPEED;
                break;
            case direction::DOWN:
                pTileTestData->scroll.y += SCROLL_SPEED;
                break;
            case direction::LEFT:
                pTileTestData->scroll.x += SCROLL_SPEED;
                break;
            case direction::RIGHT:
                pTileTestData->scroll.x -= SCROLL_SPEED;
                break;
        }

        pLayer->scroll(pTileTestData->scroll.x, pTileTestData->scroll.y);

        if (!pTileTestData->built) {            
            auto pCtx = pScene->getContext();
            auto pTiledImage = reinterpret_cast<engine::layers::tiled_image * > (pCtx->getRenderableImage(0));

            pTiledImage->invalidate();

            auto pTileSlots = *pTileTestData->ppTileSlots;
            auto& info = pTiledImage->getInfo();

            for (unsigned int j = 0; j < TILES_DOWN; j++) {
                for (unsigned int i = 0; i < TILES_ACROSS; i++) {
                    auto idx = info.index(i, j);
                    auto tile = tileDist(mt);

                    if (tile) {
                        pTileSlots[idx].view = pTileTestData->tiles[tile - 1];
                    }                    
                }
            }            

            pTileTestData->built = true;            
        }        
    });

    engine::application::setOnInit([](auto pUserData) {
        auto pScene = engine::application::getScene();
        auto pCtx = pScene->getContext();
        auto pTileTestData = reinterpret_cast<tile_test_data * > (pUserData);
        auto pTiledImage = reinterpret_cast<engine::layers::tiled_image * > (pCtx->getRenderableImage(0));

        pTileTestData->ppTileSlots = pTiledImage->fetchTileSlots();
        pTileTestData->tiles[0] = pTiledImage->getImageView(0);
        pTileTestData->tiles[1] = pTiledImage->getImageView(1);
        pTileTestData->built = false;
        pTileTestData->scroll.dir = direction::RIGHT;
        pTileTestData->scroll.x = 0.0F;
        pTileTestData->scroll.y = 0.0F;
    });

    engine::application::setUserData(std::make_shared<tile_test_data>());

    engine::application::setScene(sceneInfo);

    engine::application::start();

    return 0;
}