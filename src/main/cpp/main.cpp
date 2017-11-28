#include "app.hpp"

#include <functional>

#include "test/sound.hpp"

#ifndef FN_INIT
std::shared_ptr<void> dummyInit() {}
#define FN_INIT dummyInit
#endif

#ifndef FN_FRAME
void dummyFrame(void *) {}
#define FN_FRAME dummyFrame
#endif

constexpr int width = 640;
constexpr int height = 480;

int main(int argc, char** argv) {	
	application app(width, height, "ws"); 		
	
	app.userData = FN_INIT();
	app.start(FN_FRAME);

	return 0;
}
