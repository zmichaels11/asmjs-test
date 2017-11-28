#include "app.hpp"

#include <cstdio>

#include <iostream>
#include <memory>
#include <string>

#include "audio/sound.hpp"
#include "graphics/clear_state_info.hpp"
#include "math/mat4.hpp"
#include "math/vec4.hpp"

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

	math::vec4 pos(1.0F, 2.0F, 3.0F, 4.0F);

	auto pPos = reinterpret_cast<float *> (&pos);

	printf("<%f, %f, %f, %f>\n", pPos[0], pPos[1], pPos[2], pPos[3]);

	auto view = math::mat4::translation(pos);
	auto pView = reinterpret_cast<float *> (&view);

	printf("<%f, %f, %f, %f> <%f, %f, %f, %f> <%f, %f, %f, %f> <%f, %f, %f, %f>\n", 
	pView[0], pView[1], pView[2], pView[3], 
	pView[4], pView[5], pView[6], pView[7], 
	pView[8], pView[9], pView[10], pView[11],
	pView[12], pView[13], pView[14], pView[15]);

	app.userData = userData;	
	app.start(frame);

	return 0;
}
