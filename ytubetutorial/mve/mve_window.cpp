//
// Created by pmrj on 23-12-2024.
//

#include "mve_window.h"

#include <stdexcept>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3native.h>

namespace mve {

    MveWindow::MveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        initWindow();
    }

    void MveWindow::initWindow() {
         glfwInit();
         glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
         glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
         glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

         GLFWmonitor* monitor = glfwGetPrimaryMonitor();
         window = glfwCreateWindow(width, height, windowName.c_str(), monitor, nullptr);
         glfwMakeContextCurrent(window);
     }

     MveWindow::~MveWindow() {
         glfwDestroyWindow(window);
         glfwTerminate();
     }

    bool MveWindow::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void MveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

    VkExtent2D MveWindow::getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

    GLFWwindow* MveWindow::getSurface() {
        return window;
    }

} // namespace mve
