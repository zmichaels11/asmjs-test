#include <cstdio>

#include <memory>

#include "audio.hpp"
#include "engine.hpp"

constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;
constexpr auto HINTS = engine::application_hint::VSYNC | engine::application_hint::DEBUG;

struct sound_test_data {
	std::unique_ptr<audio::sound> sound;
};

int main(int argc, char** argv) {
	engine::application::init({"Sound Test", {SCREEN_WIDTH, SCREEN_HEIGHT}, {4, 5}, HINTS});	
	
	engine::application::setOnInit([](auto pUserData) {
		auto pSoundTestData = reinterpret_cast<sound_test_data * > (pUserData);

		pSoundTestData->sound = std::make_unique<audio::sound>("data/audio/atpcm16.wav");
		pSoundTestData->sound->play();
	});

	engine::application::setOnFrame([](auto pUserData) {
		auto pSoundTestData = reinterpret_cast<sound_test_data * > (pUserData);

		pSoundTestData->sound->onFrame();
	});

	engine::application::setUserData(std::make_shared<sound_test_data>());

	engine::application::setScene({});

	engine::application::start();

	return 0;
}

