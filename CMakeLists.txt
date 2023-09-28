cmake_minimum_required(VERSION 3.25)
project(AudioVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_subdirectory(src/audio_visualizer)
add_subdirectory(src/gl_abstractions)
