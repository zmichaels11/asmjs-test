#include "app.hpp"

#include <GLES3/gl3.h>

#include "audio/wave_file_channel.hpp"

constexpr int width = 640;
constexpr int height = 480;

void frame() {
	static bool runOnce = true;

	if (runOnce) {
		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version: %s\n", glGetString(GL_VERSION));
		printf("Extensions: %s\n", glGetString(GL_EXTENSIONS));

		runOnce = false;
	}

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char** argv) {
	application app(width, height, "ws"); 

	audio::wave_file_channel wfc("data/audio/atpcmalaw.wav");

	printf("Channels: %d\n", wfc.getChannels());
	printf("Sample Rate: %d\n", wfc.getSampleRate());
	printf("Byte Rate: %d\n", wfc.getByteRate());
	printf("Bits per sample: %d\n", wfc.getBitsPerSample());

	app.start(frame);

	return 0;
}
