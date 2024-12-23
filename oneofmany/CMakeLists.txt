cmake_minimum_required(VERSION 3.30)
project(oneofmany)

set(CMAKE_CXX_STANDARD  20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_USE_WAYLAND ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

find_library(GLFW glfw)
find_library(DL libdl)
find_library(PTHREAD pthread)


add_executable(${PROJECT_NAME}
        src/def.h)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan ${GLFW} ${dl} ${PTHREAD} ${taskflow} ${LVK})
