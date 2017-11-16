#include "app.hpp"

#include <cstdio>

#include <iostream>
#include <memory>
#include <string>

#include "audio/sound.hpp"
#include "graphics/clear_state_info.hpp"

constexpr int width = 640;
constexpr int height = 480;

struct AppData {
	std::unique_ptr<audio::sound> sound;
	graphics::clear_state_info clearState;
};

void frame(void * pUserData) {
	static bool runOnce = true;
	static int sampleSize;
	static bool eof = false;	
	auto pAppData = reinterpret_cast<AppData *> (pUserData);	

	if (runOnce) {		
		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version: %s\n", glGetString(GL_VERSION));
		printf("Extensions: %s\n", glGetString(GL_EXTENSIONS));

		pAppData->sound->play();

		runOnce = false;
	}

	pAppData->sound->onFrame();

	graphics::apply(pAppData->clearState);
}

int main(int argc, char** argv) {	
	application app(width, height, "ws"); 		
	
	auto userData = std::make_shared<AppData>();
	
	userData->sound = std::make_unique<audio::sound>("data/audio/atmono.ogg");
	userData->clearState.buffers = graphics::clear_buffer::COLOR;
	userData->clearState.color = {0.0F, 0.0F, 0.2F, 1.0F};

	app.userData = userData;	
	app.start(frame);

	return 0;
}
