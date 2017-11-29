#include <cstdio>

#include <memory>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>

#include "audio/sound.hpp"
#include "graphics/clear_state_info.hpp"
#include "engine/application.hpp"

struct AppData {
	std::unique_ptr<audio::sound> sound;
	graphics::clear_state_info clearState;
};

void frame(engine::application * pApp) {
	static bool runOnce = true;	
	auto pAppData = reinterpret_cast<AppData *> (pApp->userData.get());	

	if (runOnce) {
        printf("Vendor: %s\n", glGetString(GL_VENDOR));
        printf("Renderer: %s\n", glGetString(GL_RENDERER));
        printf("Version: %s\n", glGetString(GL_VERSION));
        printf("Extensions: %s\n", glGetString(GL_EXTENSIONS));

        pAppData->sound->play();

        runOnce = false;
    }	

	auto keyReturn = pApp->getButton(engine::input_type::KEYBOARD, GLFW_KEY_ENTER);
	
	if (keyReturn.value > 0.0 && pAppData->sound->getState() == audio::sound_state::STOPPED) {
		printf("Enter pressed at: %f\n", keyReturn.time);
		pAppData->sound = std::make_unique<audio::sound>("data/audio/atmono.ogg");		
		pAppData->sound->play();
	}

    pAppData->sound->onFrame();
    graphics::apply(pAppData->clearState);
}

int main(int argc, char** argv) {
	engine::application app(640, 480, "Sound Test");

	auto userData = std::make_shared<AppData>();

	userData->sound = std::make_unique<audio::sound>("data/audio/atmono.ogg");
	userData->clearState.buffers = graphics::clear_buffer::COLOR;
	userData->clearState.color = {0.0F, 0.7F, 0.3F, 1.0F};

	app.userData = userData;
	app.start(frame);

	return 0;
}

