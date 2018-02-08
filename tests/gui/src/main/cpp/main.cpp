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
        constexpr auto WINDOW_FLAGS = engine::layers::nuklear::panel_flags::BORDER
            | engine::layers::nuklear::panel_flags::NO_SCROLLBAR
            | engine::layers::nuklear::panel_flags::MOVABLE;

        auto pScene = engine::application::getScene();
        auto pLayer = dynamic_cast<engine::layers::gui_layer *> (pScene->getLayer(0));

        if (pLayer->begin("GUI Test", {10.0F, 10.0F, 180.0F, 250.0F}, WINDOW_FLAGS)) {

        }

        pLayer->end();
    });

    engine::application::start();

    return 0;
}