cmake_minimum_required(VERSION 3.25)
project(AudioVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# TODO : release a stable version of the
# ECS without all the C++ modules.
# Bringing the ECS through the git repo
include(FetchContent)
FetchContent_Declare(
    maecs
    GIT_REPOSITORY "https://github.com/matheusgomes28/maecs"
    GIT_TAG "155d54edf2f8b8c6807c3f89f2f09a15d1e58075"
)
FetchContent_MakeAvailable(maecs)


find_package(Qt6 REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src/audio_visualizer)
add_subdirectory(src/audio_playback)
add_subdirectory(src/gl_abstractions)
add_subdirectory(src/qt_abstractions)


#add_subdirectory(src/test)
