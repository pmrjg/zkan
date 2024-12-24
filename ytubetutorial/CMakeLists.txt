cmake_minimum_required(VERSION 3.30)
project(ytubetutorial)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_USE_WAYLAND ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(SDL3 REQUIRED)

find_library(GLFW glfw3)
find_library(IMGUI imgui)
find_library(GLM glm)
find_library(PTHREAD pthread)
find_library(SDL3 sdl3)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(${PROJECT_NAME} main.cpp
        mve/mve_window.cpp
        mve/mve_window.h
        first_app.cpp
        first_app.h
        mve/mve_Pipeline.cpp
        mve/mve_Pipeline.h
        mve/mve_device.cpp
        mve/mve_device.h
        mve/mve_swap_chain.cpp
        mve/mve_swap_chain.h
)
target_include_directories(${PROJECT_NAME} PRIVATE libs)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE dl)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan ${GLM} ${IMGUI} ${GLFW} ${PTHREAD} SDL3::SDL3 GPUOpen::VulkanMemoryAllocator)