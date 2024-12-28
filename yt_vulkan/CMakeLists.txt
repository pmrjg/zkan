cmake_minimum_required(VERSION 3.30)
project(yt_vulkan)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_USE_WAYLAND ON)

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(glslang REQUIRED)

find_library(GLFW glfw)
find_library(DL libdl)
find_library(PTHREAD pthread)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL ${GLFW} ${dl} ${PTHREAD} glm::glm imgui::imgui glslang::glslang)

