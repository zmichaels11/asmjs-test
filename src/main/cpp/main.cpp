#include "app.hpp"

#include <GLES3/gl3.h>

#include <cstdio>

#include <iostream>
#include <memory>
#include <string>

#include "audio/buffer.hpp"
#include "audio/source.hpp"
#include "audio/wave_file_channel.hpp"
#include "audio/vorbis_file_channel.hpp"

constexpr int width = 640;
constexpr int height = 480;

struct AppData {
	audio::buffer buffers[3];
	audio::source source;
	audio::wave_file_channel sound;
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

		printf("ByteRate: %d\n", pAppData->sound.getByteRate());	
		printf("SampleRate: %d\n", pAppData->sound.getSampleRate());	
		printf("Format: %d\n", static_cast<int> (pAppData->sound.getFormat()));
		sampleSize = pAppData->sound.getByteRate() / 15;

		for (int i = 0; i < 3; i++) {
			char transfer[sampleSize];
			std::size_t size = sampleSize;
			eof = !pAppData->sound.read(transfer, size);

			pAppData->buffers[i].setData(pAppData->sound.getFormat(), transfer, size, pAppData->sound.getSampleRate());			
			pAppData->source.queueBuffer(pAppData->buffers[i]);

			if (eof) {
				break;
			}
		}

		pAppData->source.play();					

		runOnce = false;
	}

	std::vector<audio::buffer> readyBuffers;

	if (!eof && pAppData->source.unqueueBuffers(readyBuffers)) {		
		for (auto&& buffer : readyBuffers) {
			char transfer[sampleSize];
			std::size_t size = sampleSize;

			if (!pAppData->sound.read(transfer, size)) {
				eof = true;
			}			

			buffer.setData(
				pAppData->sound.getFormat(),
				transfer,
				size,
				pAppData->sound.getSampleRate());

			pAppData->source.queueBuffer(buffer);			

			if (eof) {				
				break;
			}
		}
	}	

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	
}

int main(int argc, char** argv) {	
	application app(width, height, "ws"); 	

	audio::vorbis_file_channel vfc("data/audio/atmono.ogg");

	printf("vorbis channels: %d\n", vfc.getChannels());
	printf("vorbis sample rate: %d\n", vfc.getSampleRate());
	
	auto userData = std::make_shared<AppData>();

	userData->sound = audio::wave_file_channel("data/audio/atpcm16.wav");

	app.userData = userData;	
	app.start(frame);

	return 0;
}
