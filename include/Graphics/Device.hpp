#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstdint>
#include <optional>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace FFL {

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;

	bool isComplete() const {return graphicsFamily.has_value();}
};

class Device {
public:
	Device();
	~Device();

	VkPhysicalDeviceProperties properties;
private:
	VkInstance mInstance;

	VkDebugUtilsMessengerEXT mDebugMessenger;

	VkPhysicalDevice mPhysicalDevice;

	void createInstance();
	std::vector<const char*> getRequiredExtensions();
	void checkExtensionsSupport();
	void checkLayersSupport();
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& pCreateInfo);
	void pickPhysicalDevice();
	int ratePhysicalDeviceSuitability(const VkPhysicalDevice pPhysicalDevice);
	QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice pPhysicalDevice);
};

}

#endif
