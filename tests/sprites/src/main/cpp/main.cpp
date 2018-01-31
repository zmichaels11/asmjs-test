#include <cstdio>

#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "engine/application.hpp"

#include "graphics/image.hpp"

#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"
#include "engine/layers/sprite_sheet_info.hpp"

int main(int argc, char** argv) {
    engine::application::init({"Background Test", {640, 480}, {1, 0}, engine::application_hint::VSYNC});
            
    auto pSpriteImages = std::vector<std::unique_ptr<graphics::image>>();
    auto ppSpriteImages = std::vector<graphics::image*>();

    for (int i = 0; i < 10; i++) {
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

    auto sceneInfo = engine::layers::scene_info{
        {
            pSpritesheetInfos.data(), pSpritesheetInfos.size(), 
            nullptr, 0, 
            nullptr, 0},
            
        pLayerInfos.data(), pLayerInfos.size()};
    
    engine::application::setScene(sceneInfo);    

    engine::application::setOnUpdate([](auto userData) {
        static float timestep = 0.0;     
        
        timestep += 0.005F;
    });

    engine::application::setOnFrame([](auto userData) {
        
    });

    engine::application::start(nullptr);

    return 0;
}