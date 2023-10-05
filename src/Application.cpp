#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace FFL {

Application::Application() {}

Application::~Application() {}

void Application::run() {
	while(!mWindow.shouldClose()) {
		glfwPollEvents();
	}
}

}

