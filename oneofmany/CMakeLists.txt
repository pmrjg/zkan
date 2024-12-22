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

add_library(taskflow SHARED IMPORTED)
set_target_properties(taskflow PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/taskflow.hpp"
)

add_library(lvk SHARED IMPORTED)
set_target_properties(lvk PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/lvk"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include"
)

add_executable(${PROJECT_NAME}
        src/Def.cpp
        src/Def.h)
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan ${GLFW} ${dl} ${PTHREAD} ${taskflow} ${lvk})
