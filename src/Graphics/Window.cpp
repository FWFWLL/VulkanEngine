#include "Graphics/Window.hpp"

#include <stdexcept>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace FFL {

Window::Window(const uint32_t pWidth, const uint32_t pHeight, const char* pTitle) : mWidth{pWidth}, mHeight{pHeight}, mTitle{pTitle} {
	initWindow();
}

Window::~Window() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void Window::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, nullptr, nullptr);
	if(mWindow == nullptr) {
		throw std::runtime_error("Failed to create window!");
	}

	glfwSetWindowUserPointer(mWindow, this);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	if(glfwCreateWindowSurface(instance, mWindow, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface!");
	}
}

}

