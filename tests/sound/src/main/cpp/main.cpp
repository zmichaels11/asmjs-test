#include <cstdio>

#include <memory>
#include <vector>

#include "audio.hpp"
#include "engine.hpp"
#include "util.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

struct sound_test_data {
	void handleGUI(
		engine::layers::gui_layer * pGUILayer,
		engine::layers::sound_layer * pSoundLayer) noexcept;
};

int main(int argc, char** argv) {
	engine::application::init({"Sound Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {4, 5}, HINTS});

	engine::application::setOnUpdate([](auto pUserData) {
		auto pSoundTestData = reinterpret_cast<sound_test_data * > (pUserData);
		auto pScene = engine::application::getScene();
		auto pGUILayer = dynamic_cast<engine::layers::gui_layer * > (pScene->getLayer(0));
		auto pSoundLayer = dynamic_cast<engine::layers::sound_layer * > (pScene->getLayer(1));

		pSoundTestData->handleGUI(pGUILayer, pSoundLayer);
	});

	engine::application::setUserData(std::make_shared<sound_test_data>());

	auto guiInfo = engine::layers::gui_layer_info{};
	auto soundInfo = engine::layers::sound_layer_info{10};

	auto pLayerInfos = std::vector<engine::layers::scene_layer_info>();
	auto guiLayerInfo = engine::layers::scene_layer_info(guiInfo);	
	auto soundLayerInfo = engine::layers::sound_layer_info(soundInfo);

	guiLayerInfo.ext.hints = engine::layers::scene_layer_hint::CLEAR;
	guiLayerInfo.ext.clear.type = engine::layers::clear_type::COLOR;
	guiLayerInfo.ext.clear.clearColor = engine::layers::color::rgb(20, 80, 120);

	pLayerInfos.push_back(guiLayerInfo);
	pLayerInfos.push_back(soundLayerInfo);

	auto oggData = util::readAll("data/audio/atmono.ogg");
	auto soundInfos = std::vector<engine::layers::sound_info> ();
	
	soundInfos.push_back(engine::layers::sound_info::ogg(oggData.data(), oggData.size()));

	auto sceneInfo = engine::layers::scene_info{
		{
			nullptr, 0, nullptr, 
			0, nullptr,  0, 
			soundInfos.data(), soundInfos.size()},
		pLayerInfos.data(), pLayerInfos.size()};

	engine::application::setScene(sceneInfo);

	engine::application::start();

	return 0;
}

void sound_test_data::handleGUI(
	engine::layers::gui_layer * pGUILayer,
	engine::layers::sound_layer * pSoundLayer) noexcept {

	constexpr auto WINDOW_FLAGS = engine::layers::nuklear::panel_flags::BORDER
            | engine::layers::nuklear::panel_flags::MOVABLE
            | engine::layers::nuklear::panel_flags::TITLE;

	if (pGUILayer->begin("Sound Demo", {50, 50, 200, 200}, WINDOW_FLAGS)) {
		pGUILayer->layoutRowDynamic(30, 2);

		if (pGUILayer->buttonLabel("play wave")) {
			auto pSFX = std::make_unique<audio::wave_file_channel> ("data/audio/atpcm16.wav");		
			
			pSoundLayer->playSoundAt(std::move(pSFX), 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);			
			std::cout << "Playing 16bit PCM!" << std::endl;
		}

		if (pGUILayer->buttonLabel("play ogg")) {
			pSoundLayer->playSoundAt(0, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);
			
			std::cout << "Playing OGG/VORBIS!" << std::endl;
		}

		pGUILayer->layoutRowDynamic(30, 1);

		if (pGUILayer->buttonLabel("play wave (ALAW)")) {
			auto pSFX = std::make_unique<audio::wave_file_channel> ("data/audio/atpcmalaw.wav");

			pSoundLayer->playSoundAt(std::move(pSFX), 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F);

			std::cout << "Playing ALAW PCM!" << std::endl;
		}
	}

	pGUILayer->end();
}

