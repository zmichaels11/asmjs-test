#include <cstdio>

#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

constexpr int width = 640;
constexpr int height = 480;
GLFWwindow * window;

void frame() {
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main(int argc, char** argv) {
	if (glfwInit() != GL_TRUE) {
		printf("glfwInit failed!\n");
		__builtin_trap();
	}

	window = glfwCreateWindow(width, height, "ws", nullptr, nullptr);

	if (!window) {
		printf("glfwCreateWindow failed!\n");
		__builtin_trap();
	}

	glfwMakeContextCurrent(window);

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	emscripten_set_main_loop(frame, 0, 1);

	glfwTerminate();

	return 0;
}
