//
// Created by pmrj on 23-12-2024.
//

#include "vulkanengine.h"

#include <iostream>
#include <spdlog/spdlog.h>

using namespace std::string_literals;

void VulkanEngine::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH,HEIGHT,"VulkanEngine", nullptr, nullptr);
}

void VulkanEngine::initVulkan() {
    createInstance();
    setupDebugMessenger();
}

void VulkanEngine::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}


void VulkanEngine::cleanup() {
    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanEngine::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        spdlog::error("Validation layers requested, but not available!");
        throw std::runtime_error("Validation layers requested, but not available!");
    }
    VkApplicationInfo appInfo{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "VulkanEngine",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 1),
            .pEngineName = "VulkanEngine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 1),
            .apiVersion = VK_API_VERSION_1_1,
    };

    VkInstanceCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &appInfo,
    };

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // Validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // Create instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if (result != VK_SUCCESS) {
        spdlog::error("Failed to create Vulkan instance!");
        throw std::runtime_error("Failed to create Vulkan instance!");
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    spdlog::info("available extensions: \n");

    for (const auto &extension: extensions) {
        spdlog::info("\t"s + extension.extensionName + "\n");
    }
}
void VulkanEngine::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        spdlog::error("Failed to setup debug messenger!");
        throw std::runtime_error("Failed to setup debug messenger!");
    }
}

bool VulkanEngine::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) return false;
    }

    return true;
}

std::vector<const char *> VulkanEngine::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}
VkBool32 VulkanEngine::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                     const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {


    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        // Message is important enough to show
        spdlog::error("Validation layer: "s + pCallbackData->pMessage + "\n");
        std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    return VK_TRUE;


}

VkResult VulkanEngine::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanEngine::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                 const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,
        "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}
void VulkanEngine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}
