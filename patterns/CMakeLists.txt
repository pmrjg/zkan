cmake_minimum_required(VERSION 3.30)
project(patterns)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)

FetchContent_Declare(
        Boost
        GIT_REPOSITORY "https://github.com/boostorg/boost.git"
        GIT_TAG "boost-1.87.0"
        GIT_SHALLOW ON
        OVERRIDE_FIND_PACKAGE TRUE
        EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(Boost)

find_package(Boost 1.87.0 EXACT REQUIRED)


add_executable(patterns main.cpp
        src/solid_principle/single_responsibility.hpp
        src/deps/add_to_project.h
        src/solid_principle/open_closed_principle.h
        src/solid_principle/liskov_substitution_principle.h
        src/solid_principle/interface_segregation_principle.h
        src/solid_principle/dependency_inversion.h
        src/builders/builder.h
        src/builders/groovy_style.h
        src/factory/point_in_case.h
        src/factory/hot_drink.h
        src/prototype/record.h
        src/prototype/employee_factory.h
        src/prototype/serialization.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem Boost::lexical_cast Boost::serialization)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/deps/add_to_project.h")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_26)