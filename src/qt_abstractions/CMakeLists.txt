find_package(Qt6 REQUIRED)

add_library(qt_abstractions gl_window.cpp)

target_include_directories(qt_abstractions PUBLIC include)

target_link_libraries(qt_abstractions
 PRIVATE
  maecs::maecs
 PUBLIC
  gl_abstractions
  Qt6::Core
  Qt6::Gui
  Qt6::OpenGL)
