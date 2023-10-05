#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace FFL {

#ifdef NDEBUG
	const bool VALIDATION_ENABLED = false;
#else
	const bool VALIDATION_ENABLED = true;
#endif

const std::vector<const char*> REQUIRED_VALIDATION_LAYERS = {
	"VK_LAYER_KHRONOS_validation",
};

static inline VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT pMessageSeverity, VkDebugUtilsMessageTypeFlagsEXT pMessageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void*) {
	std::cerr << "[DEBUG] ";

	switch(pMessageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			std::cerr << "[VERBOSE]";
			break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			std::cerr << "[INFO]   ";
			break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			std::cerr << "[WARNING]";
			break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			std::cerr << "[ERROR]  ";
			break;
		default:
			break;
	}

	switch(pMessageType) {
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			std::cerr << " [GENERAL]    ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			std::cerr << " [VALIDATION] ";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			std::cerr << " [PERFORMANCE]";
			break;
	}

	std::cerr << ": " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

inline VkResult createDebugUtilsMessengerEXT(VkInstance pInstance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(pInstance, "vkCreateDebugUtilsMessengerEXT"));

	if(func != nullptr) {
		return func(pInstance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

inline void destroyDebugUtilsMessengerEXT(VkInstance pInstance, VkDebugUtilsMessengerEXT pDebugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(pInstance, "vkDestroyDebugUtilsMessengerEXT"));

	if(func != nullptr) {
		func(pInstance, pDebugMessenger, pAllocator);
	}
}

}

#endif

