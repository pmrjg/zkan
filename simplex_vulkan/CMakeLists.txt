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
        GIT_SHALLOW FALSE
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

#[[FetchContent_Declare(spirv-headers
        GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Headers.git"
        GIT_TAG "main"
)
FetchContent_MakeAvailable(spirv-headers)

ExternalProject_Add(
        spirv-tools
        GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Tools.git"
        GIT_TAG "v2024.2"
        GIT_SHALLOW FALSE
        BUILD_IN_SOURCE TRUE
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/SPIRV-Tools
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        INSTALL_COMMAND make DESTDIR=<INSTALL_DIR> install
)


ExternalProject_Add(
        glsld
        GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
        GIT_TAG "15.1.0"
        GIT_SHALLOW TRUE
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glslang
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        DEPENDS ${spirv-tools}
)]]

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.1.0"
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(GSL)

add_executable(${PROJECT_NAME} vk/main.cpp
        vk/glfw/glfw_initialization.h
        vk/glfw/glfw_initialization.cpp
)

#[[target_link_libraries(${PROJECT_NAME} PUBLIC ${spirv-headers})
target_link_libraries(${PROJECT_NAME} PUBLIC INTERFACE ${spirv-tools} ${glsld})]]
target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft.GSL::GSL)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_26)