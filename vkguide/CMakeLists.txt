cmake_minimum_required(VERSION 3.30)
project(vkguide)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_USE_WAYLAND ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(SDL REQUIRED)

find_library(GLFW glfw)
find_library(IMGUI imgui)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ext_libs)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan ${IMGUI} ${GLFW} ${SDL} GPUOpen::VulkanMemoryAllocator)