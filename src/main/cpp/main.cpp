#include "app.hpp"

#include <GLES3/gl3.h>

#include <cstdio>

#include <iostream>
#include <memory>
#include <string>

#include "audio/sound.hpp"

constexpr int width = 640;
constexpr int height = 480;

struct AppData {
	std::unique_ptr<audio::sound> sound;
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

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	
}

int main(int argc, char** argv) {	
	application app(width, height, "ws"); 		
	
	auto userData = std::make_shared<AppData>();
	
	userData->sound = std::make_unique<audio::sound>("data/audio/atmono.ogg");

	app.userData = userData;		
	app.start(frame);

	return 0;
}
