#include <iostream>
#include <cstdlib>
#include <cmath>
#include <gsl/gsl>
#include <cstdint>
#include <GLFW/glfw3.h>

#include "glfw/glfw_initialization.h"

std::int32_t main(std::int32_t argc, gsl::zstring* argv) {

    vkeng::GlfwInitialization _glfw;

    gsl::not_null<GLFWwindow*> window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
    gsl::final_action _cleanup_window([window] {glfwDestroyWindow(window);});

    while ( ! glfwWindowShouldClose(window) ) {
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}
