#include <GLFW/glfw3.h>

#include "glfw/glfw_initialization.h"

std::int32_t main(std::int32_t argc, gsl::zstring* argv) {

    vkeng::GlfwInitialization _glfw;

    gsl::not_null window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
    gsl::final_action _cleanup_window([window] {glfwDestroyWindow(window);});

    std::int32_t monitor_count = 0;
    GLFWmonitor** monitor_pointers = glfwGetMonitors(&monitor_count);
    gsl::span monitors (monitor_pointers, monitor_count);

    glm::ivec2 monitor_position;
    glfwGetMonitorPos(monitors[1], &monitor_position.x, &monitor_position.y);
    glfwSetWindowPos(window, monitor_position.x, monitor_position.y);


    glm::ivec2 window_size;
    glfwGetWindowSize(window, &window_size.x, &window_size.y);

    glm::ivec2 monitor_size;
    glfwGetMonitorWorkarea(monitors[1], nullptr, nullptr, &monitor_size.x, &monitor_size.y);

    glm::ivec2 window_new_position = monitor_position + monitor_size / 2 - window_size / 2;

    glfwSetWindowPos(window, window_new_position.x, window_new_position.y);

    while ( ! glfwWindowShouldClose(window) ) {
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}
