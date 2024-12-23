//
// Created by pmrj on 23-12-2024.
//

#ifndef VULKANENGINE_H
#define VULKANENGINE_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class VulkanEngine {
public:
    const int WIDTH = 800;
    const int HEIGHT = 600;
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance instance;
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
    void createInstance();
};



#endif //VULKANENGINE_H
