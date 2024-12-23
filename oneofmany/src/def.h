//
// Created by pmrj on 22-12-2024.
//

#ifndef DEF_H
#define DEF_H

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <volk/volk.h>
#include <lvk/vulkan/VulkanUtils.h>
#include <lvk/Pool.h>
#include <GLFW/glfw3.h>

class General {
public:
  static GLFWwindow* setWindow(uint32_t width, uint32_t height) {
    return nullptr;
  }
};


#endif //DEF_H
