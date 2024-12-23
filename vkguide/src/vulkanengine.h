//
// Created by pmrj on 23-12-2024.
//

#ifndef VULKANENGINE_H
#define VULKANENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <cstring>

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

class VulkanEngine {
public:
    const int WIDTH = 800;
    const int HEIGHT = 600;

    const std::pmr::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
    void createInstance();

    void setupDebugMessenger();
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger);
    static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator);
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& pCreateInfo);
};



#endif //VULKANENGINE_H
