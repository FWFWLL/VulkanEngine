#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cstdint>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace FFL {

class Window {
public:
	Window(const uint32_t pWidth, const uint32_t pHeight, const char *pTitle);
	~Window();

	bool shouldClose() const {return glfwWindowShouldClose(mWindow);}

	void createWindowSurface(VkInstance pInstance, VkSurfaceKHR *pSurface);
private:
	uint32_t mWidth;
	uint32_t mHeight;

	const char *mTitle;

	GLFWwindow *mWindow;

	void initWindow();
};

}

#endif

