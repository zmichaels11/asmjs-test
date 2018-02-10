#include <cstdio>

#include <memory>
#include <vector>

#include "audio.hpp"
#include "engine.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

struct sound_test_data {
	std::unique_ptr<audio::sound> sound;

	sound_test_data() :
		sound(nullptr) {}

	void handleGUI(engine::layers::gui_layer * pLayer) noexcept;
};

int main(int argc, char** argv) {
	engine::application::init({"Sound Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {4, 5}, HINTS});

	engine::application::setOnUpdate([](auto pUserData) {
		auto pSoundTestData = reinterpret_cast<sound_test_data * > (pUserData);

		if (pSoundTestData->sound.get()) {
			pSoundTestData->sound->onFrame();
		}

		auto pScene = engine::application::getScene();
		auto pLayer = dynamic_cast<engine::layers::gui_layer * > (pScene->getLayer(0));

		pSoundTestData->handleGUI(pLayer);
	});

	engine::application::setUserData(std::make_shared<sound_test_data>());

	auto guiInfo = engine::layers::gui_layer_info{};
	auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();
	auto guiLayerInfo = engine::layers::scene_layer_info(guiInfo);

	guiLayerInfo.ext.hints = engine::layers::scene_layer_hint::CLEAR;
	guiLayerInfo.ext.clear.type = engine::layers::clear_type::COLOR;
	guiLayerInfo.ext.clear.clearColor = engine::layers::color::rgb(20, 80, 120);

	pLayerInfos.push_back(guiLayerInfo);

	auto sceneInfo = engine::layers::scene_info{
		{nullptr, 0, nullptr, 0, nullptr,  0},
		pLayerInfos.data(), pLayerInfos.size()};

	engine::application::setScene(sceneInfo);

	engine::application::start();

	return 0;
}

void sound_test_data::handleGUI(engine::layers::gui_layer * pLayer) noexcept {
	constexpr auto WINDOW_FLAGS = engine::layers::nuklear::panel_flags::BORDER
            | engine::layers::nuklear::panel_flags::MOVABLE
            | engine::layers::nuklear::panel_flags::TITLE;

	if (pLayer->begin("Sound Demo", {50, 50, 200, 200}, WINDOW_FLAGS)) {
		pLayer->layoutRowDynamic(30, 2);

		if (pLayer->buttonLabel("load wave")) {
			sound = std::make_unique<audio::sound> ("data/audio/atpcm16.wav");
		}

		if (pLayer->buttonLabel("load ogg")) {
			sound = std::make_unique<audio::sound> ("data/audio/atmono.ogg");			
		}

		pLayer->layoutRowDynamic(30, 3);

		if (pLayer->buttonLabel("play")) {
			if (sound.get()) {
				sound->play();
			} else {
				std::cerr << "No sound is loaded!" << std::endl;
			}
		}

		if (pLayer->buttonLabel("stop")) {
			if (sound.get()) {
				sound->stop();
			} else {
				std::cerr << "No sound is loaded!" << std::endl;
			}
		}

		if (pLayer->buttonLabel("loop")) {
			if (sound.get()) {
				sound->loop();
			} else {
				std::cerr << "No sound is loaded!" << std::endl;
			}
		}
	}

	pLayer->end();
}

