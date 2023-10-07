#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Graphics/Device.hpp"
#include "Graphics/Window.hpp"

#include <cstdint>
#include <iostream>

namespace FFL {

class Application {
public:
	static constexpr uint32_t SCREEN_WIDTH = 640;
	static constexpr uint32_t SCREEN_HEIGHT = 640;

	static constexpr const char* WINDOW_TITLE = "VulkanEngine";

	Application();
	~Application();

	void run();
private:
	Window mWindow{SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE};
	Device mDevice{};
};

}

#endif

