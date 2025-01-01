cmake_minimum_required(VERSION 3.30)
project(repeat_repeat)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

include(ExternalProject)

FetchContent_Declare(
        vk
        GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Hpp.git"
        GIT_TAG "v1.4.303"
        GIT_SHALLOW FALSE
)

FetchContent_MakeAvailable(vk)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.4"
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG "1.0.1"
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.1.0"
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(GSL)

add_executable(${PROJECT_NAME} main.cpp
        vk/deps/add_to_project.h)

target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft.GSL::GSL)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/vk/deps/add_to_project.h")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_26)