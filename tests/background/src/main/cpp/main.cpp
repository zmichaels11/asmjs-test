#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <memory>
#include <vector>

#include "engine/application.hpp"

#include "graphics/image.hpp"

#include "math/mat2.hpp"
#include "math/mat4.hpp"

#include "engine/layers/background_layer.hpp"
#include "engine/layers/background_layer_info.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"

int main(int argc, char** argv) {
    engine::application::init({"Background Test", {640, 480}, {1, 0}, true});

    auto backgroundImgRes = graphics::image::read("data/images/environment.png", 4);    
    auto backgroundInfo = engine::layers::background_layer_info{
        backgroundImgRes.get(),
        {engine::layers::image_scroll_type::STATIC, engine::layers::image_scroll_type::STATIC}};

    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();

    pLayerInfos.push_back(engine::layers::scene_layer_info::init(backgroundInfo));

    auto sceneInfo = engine::layers::scene_info{
        {0},
        pLayerInfos.data(), pLayerInfos.size()};
    
    engine::application::setScene(sceneInfo);

    engine::application::setOnUpdate([](auto userData) {
        static float timestep = 0.0;

        auto pScene = engine::application::getScene();
        auto pLayer = dynamic_cast<engine::layers::background_layer * > (pScene->getLayer(0));

        auto h = 0.5F * std::sin(timestep);
        auto v = 0.5F * std::cos(timestep);
        auto s = 0.5F * std::sin(timestep) + 0.5F;        

        //pLayer->scroll(0.5F + h, 0.5F + v);
        //pLayer->setTransform(s, 0.0F, 0.0F, s);
        pLayer->setTransform(math::mat2::rotate(timestep) * s);
        pLayer->setOrigin(0.5F, 0.5F);

        timestep += 0.005F;
    });

    engine::application::setOnFrame([](auto userData) {
        
    });

    engine::application::start(nullptr);

    return 0;
}