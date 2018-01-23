#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <memory>
#include <vector>

#include "engine/application.hpp"

#include "graphics/image.hpp"

#include "math/mat4.hpp"

#include "engine/layers/basic_image_layer_info.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"

#include "nk/button.hpp"
#include "nk/edit.hpp"
#include "nk/layout.hpp"
#include "nk/window.hpp"

struct AppData {
    struct calculator_t {
        int prev, op;
        bool set;
        double a, b;
        double * current;        
    } calculator;
};

std::shared_ptr<AppData> _userData;

int main(int argc, char** argv) {
    engine::application::init({"Calculator", {640, 480}, {4, 5}, true});

    auto backgroundImgRes = graphics::image::read("data/images/environment.png", 4);    
    auto backgroundInfo = engine::layers::basic_image_layer_info{
        backgroundImgRes.get(),
        {engine::layers::image_scroll_type::STATIC, engine::layers::image_scroll_type::STATIC}};

    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();

    pLayerInfos.push_back(engine::layers::scene_layer_info::init(backgroundInfo));

    auto sceneInfo = engine::layers::scene_info{
        {0},
        pLayerInfos.data(), pLayerInfos.size()};
    
    engine::application::setScene(sceneInfo);

    auto userData = std::make_shared<AppData>();
    
    userData->calculator.current = &userData->calculator.a;

    engine::application::setOnUpdate([](auto userData) {
        auto appData = reinterpret_cast<AppData*> (userData);
        auto& calc = appData->calculator;
        auto pScene = engine::application::getScene();
        
    });

    engine::application::setOnFrame([](auto userData) {
        
    });

    engine::application::start(userData);

    return 0;
}