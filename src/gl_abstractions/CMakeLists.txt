find_package(glew REQUIRED)

add_library(gl_abstractions shaders.cpp)
target_include_directories(gl_abstractions PUBLIC include)

target_link_libraries(gl_abstractions PRIVATE GLEW::GLEW)
