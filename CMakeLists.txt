cmake_minimum_required(VERSION 3.10)

project(CppGameTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
include(FetchContent)
include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_IMAGE REQUIRED SDL2_image)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

# FetchContent_MakeAvailable(Catch2)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

include_directories(${VULKAN_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(CppGameTest ${SOURCES})
target_link_libraries(CppGameTest ${SDL2_LIBRARIES})
target_link_libraries(CppGameTest ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(CppGameTest ${SDL2_TTF_LIBRARIES})

# file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
# list(APPEND TEST_SOURCES ${SOURCES})
# list(REMOVE_ITEM TEST_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)
# add_executable(CppGameTestTest ${TEST_SOURCES})
# target_link_libraries(CppGameTestTest ${SDL2_LIBRARIES})
# target_link_libraries(CppGameTestTest ${SDL2_IMAGE_LIBRARIES})
# target_link_libraries(CppGameTestTest Catch2::Catch2WithMain)

