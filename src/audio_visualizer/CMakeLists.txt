find_package(audiofile REQUIRED)
find_package(fmt REQUIRED)
find_package(Qt6 REQUIRED)

add_executable(visualizer main.cpp)

target_link_libraries(visualizer
 PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Multimedia
  audio_playback
  fmt::fmt
  gl_abstractions
  maecs::maecs
  qt_abstractions
)

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
