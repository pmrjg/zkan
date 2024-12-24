//
// Created by pmrj on 23-12-2024.
//

#include "mve_window.h"

#include <stdexcept>

namespace mve {

    MveWindow::MveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        window = initWindow();
    }

    GLFWwindow* MveWindow::initWindow() const {
         glfwInit();
         glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
         glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

         return glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
     }

     MveWindow::~MveWindow() {
         glfwDestroyWindow(window);
         glfwTerminate();
     }

    bool MveWindow::shouldClose() const { return glfwWindowShouldClose(window); }

    void MveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) const {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

} // namespace mve
