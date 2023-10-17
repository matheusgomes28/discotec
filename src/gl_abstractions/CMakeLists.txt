find_package(Qt6 REQUIRED)

add_library(gl_abstractions
  shaders.cpp
  renderer.cpp)
target_include_directories(gl_abstractions PUBLIC include)

target_link_libraries(gl_abstractions
 PUBLIC
  Qt6::Core
  Qt6::OpenGL)
