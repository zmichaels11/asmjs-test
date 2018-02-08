#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <memory>
#include <vector>

#include "audio.hpp"
#include "graphics.hpp"
#include "engine.hpp"

#include "calculator.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

struct gui_test_data {
    std::vector<std::unique_ptr<gui_test::gui_component>> components;
};

int main(int argc, char** argv) {
    engine::application::init({"GUI Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {1, 0}, HINTS});

    auto guiInfo = engine::layers::gui_layer_info {};
    auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();
    auto guiLayerInfo = engine::layers::scene_layer_info(guiInfo);

    guiLayerInfo.ext.hints = engine::layers::scene_layer_hint::CLEAR;
    guiLayerInfo.ext.clear.type = engine::layers::clear_type::COLOR,
    guiLayerInfo.ext.clear.clearColor = engine::layers::color::rgb(20, 80, 120);

    pLayerInfos.push_back(guiLayerInfo);

    auto sceneInfo = engine::layers::scene_info {
        {nullptr, 0, nullptr, 0, nullptr, 0},
        pLayerInfos.data(), pLayerInfos.size()};    

    engine::application::setOnUpdate([](auto pUserData) {
        auto pGuiTestData = reinterpret_cast<gui_test_data * > (pUserData);
        auto pScene = engine::application::getScene();
        auto pLayer = dynamic_cast<engine::layers::gui_layer *> (pScene->getLayer(0));

        for (auto&& component : pGuiTestData->components) {
            component->handle(pLayer);
        }
    });

    auto userData = std::make_shared<gui_test_data> ();

    userData->components.push_back(std::make_unique<gui_test::calculator>());    

    engine::application::setUserData(userData);

    engine::application::setScene(sceneInfo);

    engine::application::start();

    return 0;
}