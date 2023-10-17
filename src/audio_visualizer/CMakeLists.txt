find_package(audiofile REQUIRED)
find_package(fmt REQUIRED)
find_package(Qt6 REQUIRED)

add_executable(visualizer main.cpp)

target_link_libraries(visualizer
 PRIVATE
  fmt::fmt
  gl_abstractions
  qt_abstractions
  Qt6::Core
  Qt6::Gui
  Qt6::Multimedia
  
  audio_playback)

if (WIN32)
  target_link_libraries(visualizer
   PRIVATE
    Qt6::QWindowsIntegrationPlugin
    Dcomp.lib
    shcore.lib
    d3d9.lib
    SetupApi.lib)

  target_compile_definitions(visualizer PRIVATE WINDOWS)
endif()

include(GNUInstallDirs)
