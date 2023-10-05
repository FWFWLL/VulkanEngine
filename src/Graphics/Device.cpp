#include "Graphics/Device.hpp"
#include "Utility/Debug.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace FFL {

Device::Device() {
	createInstance();
	createDebugMessenger();
}

Device::~Device() {
	if(VALIDATION_ENABLED) {
		destroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
	}

	vkDestroyInstance(mInstance, nullptr);
}

void Device::createInstance() {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "VulkanEngine";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
	appInfo.pEngineName = "VulkanEngine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;

	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = {};
	if(VALIDATION_ENABLED) {
		checkLayersSupport();

		createInfo.enabledLayerCount = static_cast<uint32_t>(REQUIRED_VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = REQUIRED_VALIDATION_LAYERS.data();

		populateDebugMessengerCreateInfo(debugMessengerCreateInfo);
		createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugMessengerCreateInfo);
	} else {
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		
		createInfo.pNext = nullptr;
	}

	std::vector<const char*> requiredExtensions = getRequiredExtensions();
	if(requiredExtensions.size() > 0) {
		checkExtensionsSupport();

		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
	} else {
		createInfo.enabledExtensionCount = 0;
		createInfo.ppEnabledExtensionNames = nullptr;
	}

	if(vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance!");
	}
}

std::vector<const char*> Device::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if(VALIDATION_ENABLED) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void Device::checkExtensionsSupport() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

	std::unordered_set<std::string> available;

	std::cout << "Available Extensions:\n";
	for(const auto& extension : availableExtensions) {
		std::cout << '\t' << extension.extensionName << '\n';

		available.insert(extension.extensionName);
	}

	std::vector<const char*> requiredExtensions = getRequiredExtensions();

	std::cout << "Required Extensions:\n";
	for(const auto& extensionName : requiredExtensions) {
		std::cout << '\t' << extensionName << '\n';

		if(available.find(extensionName) == available.end()) {
			throw std::runtime_error("Missing required extension!");
		}
	}
}

void Device::checkLayersSupport() {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	std::unordered_set<std::string> available;

	std::cout << "Available Layers:\n";
	for(const auto& layer : availableLayers) {
		std::cout << '\t' << layer.layerName << '\n';

		available.insert(layer.layerName);
	}

	std::cout << "Required Layers:\n";
	for(const char* layerName : REQUIRED_VALIDATION_LAYERS) {
		std::cout << '\t' << layerName << '\n';

		if(available.find(layerName) == available.end()) {
			throw std::runtime_error("Missing required layer!");
		}
	}
}

void Device::createDebugMessenger() {
	if(!VALIDATION_ENABLED) {
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	populateDebugMessengerCreateInfo(createInfo);

	if(createDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create debug messenger!");
	}
}

void Device::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& pCreateInfo) {
	pCreateInfo = {};
	pCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    pCreateInfo.pNext = nullptr;
    pCreateInfo.flags = 0;
    pCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    pCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    pCreateInfo.pfnUserCallback = debugCallback;
    pCreateInfo.pUserData = nullptr;
}

}

