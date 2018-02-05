#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <memory>
#include <vector>

#include "engine/application.hpp"

#include "graphics/image.hpp"

#include "math/mat2.hpp"

#include "engine/layers/background_layer.hpp"
#include "engine/layers/background_layer_info.hpp"
#include "engine/layers/renderable_info.hpp"
#include "engine/layers/scene_info.hpp"
#include "engine/layers/scene_layer_info.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

int main(int argc, char** argv) {
    engine::application::init({"Tiles Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {1, 0}, HINTS});
        
    

    auto sceneInfo = engine::layers::scene_info{
        {nullptr, 0, nullptr, 0, nullptr, 0},
        nullptr, 0};
    
    engine::application::setScene(sceneInfo);    

    engine::application::setOnUpdate([](auto userData) {
                
    });

    engine::application::start();

    return 0;
}