find_package(audiofile REQUIRED)
find_package(Qt6 REQUIRED)

add_library(audio_playback 
  audio_device.cpp
  audio_file.cpp
  audio_player.cpp)

target_include_directories(audio_playback PUBLIC include)

target_link_libraries(audio_playback
 PRIVATE
  audiofile::audiofile
)

target_link_libraries(audio_playback
 PUBLIC
  Qt6::Multimedia)
