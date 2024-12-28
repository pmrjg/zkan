//
// Created by pmrj on 28-12-2024.
//
#include "glfw_initialization.h"

#include <GLFW/glfw3.h>


namespace vkeng {
    GlfwInitialization::GlfwInitialization() {
        if (glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
        if (glfwInit() != GLFW_TRUE) {
            std::exit(EXIT_FAILURE);
        }
    }

    GlfwInitialization::~GlfwInitialization() {
        glfwTerminate();
    }


}