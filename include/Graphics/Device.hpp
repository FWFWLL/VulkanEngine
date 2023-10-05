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

	VkDebugUtilsMessengerEXT mDebugMessenger;

	void createInstance();
	std::vector<const char*> getRequiredExtensions();
	void checkExtensionsSupport();
	void checkLayersSupport();
	void createDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& pCreateInfo);
};

}

#endif
