#include <cstdio>

#include <memory>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

#include "math/vec2.hpp"
#include "math/mat4.hpp"

#include "engine/application.hpp"

#include "graphics/image.hpp"

#include "engine/layers/basic_sprite_layer.hpp"
#include "engine/layers/basic_sprite_layer_info.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"
#include "engine/layers/sprite_sheet_info.hpp"

constexpr unsigned int MAX_SPRITES = 256;
constexpr unsigned int FRAME_COUNT = 10;
constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr float SPRITE_WIDTH = 32.0F;
constexpr float SPRITE_HEIGHT = 64.0F;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

struct sprite_test_data {
    engine::layers::basic_sprite_slot ** ppSpriteSlots;
    engine::layers::image_view frames[FRAME_COUNT];

    struct sprite_data_t {
        math::vec2 pos, vel;
        int frameID;        
    } sprites[MAX_SPRITES];
};

int main(int argc, char** argv) {
    engine::application::init({"Background Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {1, 0}, HINTS});
            
    auto spriteCache = std::vector<decltype(graphics::image::read(""))>();
    auto ppSpriteImages = std::vector<graphics::image*>();

    for (unsigned int i = 0; i < FRAME_COUNT; i++) {
        auto imgName = std::stringstream();

        imgName << "data/images/duke/duke" << i << ".png";

        auto img = graphics::image::read(imgName.str());

        ppSpriteImages.push_back(img.get());
        spriteCache.push_back(std::move(img));
    }

    auto pSpritesheetInfos = std::vector<engine::layers::sprite_sheet_info>();

    pSpritesheetInfos.push_back({
        ppSpriteImages.data(), ppSpriteImages.size(),
        engine::layers::sprite_sheet_hint::LAYERED
    });

    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();
    auto spriteInfo = engine::layers::basic_sprite_layer_info{MAX_SPRITES, engine::layers::write_hint::OFTEN, 0};    
    auto spriteLayerInfo = engine::layers::scene_layer_info::init(spriteInfo);

    spriteLayerInfo.ext = {
        engine::layers::scene_layer_hint::CLEAR,
        {
            engine::layers::clear_type::COLOR,
            engine::layers::color::rgb(20, 80, 120)
        }};

    pLayerInfos.push_back(spriteLayerInfo);
    

    auto sceneInfo = engine::layers::scene_info{
        {
            pSpritesheetInfos.data(), pSpritesheetInfos.size(), 
            nullptr, 0, 
            nullptr, 0},

        pLayerInfos.data(), pLayerInfos.size()};

    std::random_device rd;    
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> xdist(0.0, SCREEN_WIDTH);
    std::uniform_real_distribution<double> ydist(0.0, SCREEN_HEIGHT);
    std::uniform_int_distribution<int> fdist(0, FRAME_COUNT);
    std::uniform_real_distribution<double> vdist(-1.0, 1.0);

    engine::application::setOnUpdate([&](auto pUserData) {        
        auto pSpriteTestData = reinterpret_cast<sprite_test_data * > (pUserData);
        auto pSpriteSlots = *pSpriteTestData->ppSpriteSlots;

        for (unsigned int i = 0; i < MAX_SPRITES; i++) {
            auto pSprite = pSpriteTestData->sprites + i;
            auto pSlot = pSpriteSlots + i;

            pSlot->shape = engine::layers::parallelogram<float>::quad(pSprite->pos.x, pSprite->pos.y, SPRITE_WIDTH, SPRITE_HEIGHT);
            pSlot->view = pSpriteTestData->frames[pSprite->frameID];

            pSprite->pos += pSprite->vel;
            pSprite->frameID = (pSprite->frameID+1) % FRAME_COUNT;

            if (pSprite->pos.x > static_cast<float> (SCREEN_WIDTH) 
                || pSprite->pos.x < -SPRITE_WIDTH 
                || pSprite->pos.y > static_cast<float> (SCREEN_HEIGHT)
                || pSprite->pos.y < -SPRITE_HEIGHT) {

                pSprite->pos = math::vec2(
                    static_cast<float> (xdist(mt)),
                    static_cast<float> (ydist(mt)));
            }
        }
    });

    engine::application::setOnInit([&](auto pUserData) {
        auto pScene = engine::application::getScene();
        auto pLayer = dynamic_cast<engine::layers::basic_sprite_layer * > (pScene->getLayer(0));
        auto pSpriteTestData = reinterpret_cast<sprite_test_data * > (pUserData);

        pSpriteTestData->ppSpriteSlots = pLayer->fetchSpriteSlots();

        for (unsigned int i = 0; i < FRAME_COUNT; i++) {
            auto& view = pLayer->getImageView(i);

            pSpriteTestData->frames[i] = view;
        }

        for (unsigned int i = 0; i < MAX_SPRITES; i++) {
            auto pSprite = pSpriteTestData->sprites + i;

            pSprite->pos = math::vec2(
                static_cast<float> (xdist(mt)), 
                static_cast<float> (ydist(mt)));

            pSprite->vel = math::vec2(
                static_cast<float> (vdist(mt)),
                static_cast<float> (vdist(mt)));

            pSprite->frameID = fdist(mt);
        }

        pLayer->setProjection(math::mat4::ortho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1));
    });    

    engine::application::setUserData(std::make_shared<sprite_test_data> ());

    engine::application::setScene(sceneInfo);

    engine::application::start();

    return 0;
}