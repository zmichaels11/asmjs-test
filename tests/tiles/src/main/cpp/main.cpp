#include "engine/application.hpp"

#include <string>

#include "graphics/image.hpp"

#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"
#include "engine/layers/sprite_sheet_info.hpp"
#include "engine/layers/tiled_image.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

auto _loadTile(
    std::vector<graphics::image * >& imgRefs,
    const std::string& res) noexcept -> decltype(graphics::image::read("")) {

    auto ptr = graphics::image::read(res);

    imgRefs.push_back(ptr.get());

    return ptr;    
}

struct tile_test_data {
    engine::layers::tile_slot ** ppTileSlots;
    engine::layers::image_view tiles[2];    
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

    pLayerInfos.push_back(engine::layers::scene_layer_info::init(backgroundInfo));

    auto pRenderableInfos = std::vector<engine::layers::renderable_info>();
    auto tiledBackgroundInfo = engine::layers::renderable_info();

    tiledBackgroundInfo.type = engine::layers::renderable_type::TILED_IMAGE;
    tiledBackgroundInfo.info.tiledImageInfo = {
        .dim = {32, 32},
        .tileSize = {32, 32},
        .scroll = {engine::layers::image_scroll_type::REPEAT, engine::layers::image_scroll_type::REPEAT},
        .filter = engine::layers::image_filter_type::BILINEAR,
        .clearColor = engine::layers::color::rgb(10, 20, 80),
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
    
    engine::application::setScene(sceneInfo);    

    engine::application::setOnUpdate([](auto userData) {
                
    });

    engine::application::setOnInit([](auto pUserData) {
        auto pScene = engine::application::getScene();
        auto pCtx = pScene->getContext();
        auto pTileTestData = reinterpret_cast<tile_test_data * > (pUserData);
        auto pTiledImage = reinterpret_cast<engine::layers::tiled_image * >(pCtx->getRenderableImage(0));

        pTileTestData->ppTileSlots = pTiledImage->fetchTileSlots();
        pTileTestData->tiles[0] = pTiledImage->getImageView(0);
        pTileTestData->tiles[1] = pTiledImage->getImageView(1);
    });

    engine::application::start();

    return 0;
}