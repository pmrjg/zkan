//
// Created by pmrj on 23-12-2024.
//

#include "vulkanengine.h"
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
}

void VulkanEngine::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}


void VulkanEngine::cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanEngine::createInstance() {
    VkApplicationInfo appInfo{
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "VulkanEngine",
    .applicationVersion = VK_MAKE_VERSION(1,0,1),
    .pEngineName = "VulkanEngine",
    .engineVersion = VK_MAKE_VERSION(1,0,1),
    .apiVersion = VK_API_VERSION_1_1,
    };

    VkInstanceCreateInfo createInfo{
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    };

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // Validation layers
    createInfo.enabledLayerCount = 0;

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

    for (const auto& extension: extensions) {
        spdlog::info("\t"s + extension.extensionName + "\n");
    }
}
