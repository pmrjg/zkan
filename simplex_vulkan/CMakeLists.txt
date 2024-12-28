cmake_minimum_required(VERSION 3.30)
project(simplex_vulkan)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        vulkan
        GIT_REPOSITORY "https://github.com/KhronosGroup/Vulkan-Hpp"
        GIT_TAG v1.4.303
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(vulkan)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw"
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm"
        GIT_TAG 1.0.1
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)



file(GLOB_RECURSE CODE_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE CODE_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_executable(${PROJECT_NAME} main.cpp
    ${CODE_SOURCE_FILES} ${CODE_HEADER_FILEs}
)
target_link_libraries(${PROJECT_NAME} PRIVATE vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_26)