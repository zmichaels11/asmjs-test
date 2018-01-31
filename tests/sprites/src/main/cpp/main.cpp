#include <cstdio>

#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "engine/application.hpp"

#include "graphics/image.hpp"

#include "engine/layers/basic_sprite_layer.hpp"
#include "engine/layers/basic_sprite_layer_info.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"
#include "engine/layers/sprite_sheet_info.hpp"

constexpr unsigned int MAX_SPRITES = 256;
constexpr unsigned int FRAME_COUNT = 10;

struct sprite_test_data {
    engine::layers::basic_sprite_slot ** ppSpriteSlots;
    engine::layers::image_view frames[FRAME_COUNT];
};

int main(int argc, char** argv) {
    engine::application::init({"Background Test", {640, 480}, {1, 0}, engine::application_hint::VSYNC});
            
    auto pSpriteImages = std::vector<std::unique_ptr<graphics::image>>();
    auto ppSpriteImages = std::vector<graphics::image*>();

    for (int i = 0; i < FRAME_COUNT; i++) {
        auto imgName = std::stringstream();

        imgName << "data/images/duke/duke" << i << ".png";

        auto img = graphics::image::read(imgName.str());

        ppSpriteImages.push_back(img.get());
        pSpriteImages.push_back(std::move(img));
    }

    auto pSpritesheetInfos = std::vector<engine::layers::sprite_sheet_info>();

    pSpritesheetInfos.push_back({
        ppSpriteImages.data(), ppSpriteImages.size(),
        engine::layers::sprite_sheet_hint::LAYERED
    });

    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();
    auto spriteInfo = engine::layers::basic_sprite_layer_info{256, engine::layers::write_hint::OFTEN, 0};    
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

    engine::application::setOnUpdate([](auto pUserData) {        
        static int frameID = 0;        
        
        auto pSpriteTestData = reinterpret_cast<sprite_test_data * > (pUserData);
        auto pSpriteSlots = *pSpriteTestData->ppSpriteSlots;
                
        pSpriteSlots[0].shape = {
            {16, 16},
            {48, 16},
            {16, 16 + 64}};        
        
        pSpriteSlots[0].view = pSpriteTestData->frames[frameID]; 

        frameID = (frameID + 1) % FRAME_COUNT;
    });

    engine::application::setOnInit([](auto pUserData) {
        auto pScene = engine::application::getScene();
        auto pLayer = dynamic_cast<engine::layers::basic_sprite_layer * > (pScene->getLayer(0));
        auto pSpriteTestData = reinterpret_cast<sprite_test_data * > (pUserData);

        pSpriteTestData->ppSpriteSlots = pLayer->fetchSpriteSlots();

        for (unsigned int i = 0; i < FRAME_COUNT; i++) {
            auto& view = pLayer->getImageView(i);

            pSpriteTestData->frames[i] = view;
        }

        pLayer->setProjection(math::mat4::ortho(0, 640, 480, 0, 0, 1));
    });    

    engine::application::setUserData(std::make_shared<sprite_test_data> ());

    engine::application::setScene(sceneInfo);

    engine::application::start();

    return 0;
}