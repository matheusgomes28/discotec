find_package(fmt REQUIRED)
find_package(Qt6 REQUIRED)


add_executable(visualizer main.cpp)

target_link_libraries(visualizer
 PRIVATE
  fmt::fmt
  gl_abstractions
  qt_abstractions
  Qt6::Gui)
