//
// Created by pmrj on 22-12-2024.
//

#ifndef DEF_H
#define DEF_H

#include <vulkan/vulkan.h>


#define VK_ASSERT(func) { \
  const VkResult vk_assert_result = func; \
  if (vk_assert_result != VK_SUCCESS) { \
    LLOGW("Vulkan API call failed: %s: %i\n %s\n %s\n", \
              __FILE__, __LINE__, #func, ivkGetVulkanResultString(vk_assert_result)); \
    assert(false); \
    } \
}

#define VK_ASSERT_RETURN(func) { \
  const VkResult vk_assert_result = func; \
  if (vk_assert_result != VK_SUCCESS) { \
    LLOGW("Vulkan API call failed: %s:%i\n %s\n %s\n", \
        __FILE__, __LINE__, #func, ivkGetVulkanResultString(vk_assert_result));\
  }\
}

#endif //DEF_H
