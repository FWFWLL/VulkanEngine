#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <vector>

#include <vulkan/vulkan_core.h>

namespace FFL {

class Device {
public:
	Device();
	~Device();
private:
	VkInstance mInstance;

	void createInstance();
	std::vector<const char*> getRequiredExtensions();
	void enumerateExtensions();

};

}

#endif
