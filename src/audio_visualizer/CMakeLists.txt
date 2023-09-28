find_package(fmt REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)


add_executable(visualizer main.cpp)

target_link_libraries(visualizer
 PRIVATE
 GLEW::GLEW
 fmt::fmt
 glfw
 gl_abstractions)
