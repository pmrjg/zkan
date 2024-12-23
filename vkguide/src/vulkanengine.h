//
// Created by pmrj on 23-12-2024.
//

#ifndef VULKANENGINE_H
#define VULKANENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vkbootstrap/VkBootstrap.h>
#include <vulkan/vulkan.h>


class VulkanEngine {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
};



#endif //VULKANENGINE_H
