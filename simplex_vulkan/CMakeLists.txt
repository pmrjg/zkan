cmake_minimum_required(VERSION 3.30)
project(simplex_vulkan)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

include(ExternalProject)

FetchContent_Declare(
        vulkan
        GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Hpp.git"
        GIT_TAG "v1.4.303"
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(vulkan)

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

FetchContent_Declare(
        spirv_headers
        GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Headers.git"
        GIT_TAG "1.4.2"
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(spirv_headers)

ExternalProject_Add(
        spirv_tools
        GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Tools.git"
        GIT_TAG "v2024.2"
        GIT_SHALLOW FALSE
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/spirv-tools
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)


ExternalProject_Add(
        glslangd
        GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
        GIT_TAG "15.1.0"
        GIT_SHALLOW TRUE
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glslang
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        DEPENDS ${SPIRV_TOOLS}
)

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.1.0"
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(GSL)


file(GLOB_RECURSE CODE_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE CODE_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(${PROJECT_NAME} vk/main.cpp
    ${CODE_SOURCE_FILES} ${CODE_HEADER_FILEs}
        vk/glfw/glfw_initialization.h
        vk/glfw/glfw_initialization.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glslang)
target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft.GSL::GSL)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_26)