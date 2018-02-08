#include <cstddef>

#include <memory>
#include <vector>

#include "audio.hpp"
#include "graphics.hpp"
#include "engine.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

int main(int argc, char** argv) {
    engine::application::init({"GUI Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {1, 0}, HINTS});

    auto guiInfo = engine::layers::gui_layer_info {};
    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();

    pLayerInfos.push_back(engine::layers::scene_layer_info(guiInfo));

    auto sceneInfo = engine::layers::scene_info {
        {nullptr, 0, nullptr, 0, nullptr, 0},
        pLayerInfos.data(), pLayerInfos.size()};

    engine::application::setScene(sceneInfo);

    engine::application::setOnUpdate([](auto pUserData) {

    });

    engine::application::start();

    return 0;
}