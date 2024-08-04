#ifndef AUDIO_PLAYBACK_AUDIO_PLAYER_H
#define AUDIO_PLAYBACK_AUDIO_PLAYER_H

#include <QObject>

#include <string>

QT_BEGIN_NAMESPACE
class QAudioSink;
QT_END_NAMESPACE

class WaveFile;

class AudioPlayer
{
public:
    AudioPlayer();
    bool load_wave(std::string const& filename);

private:
    std::unique_ptr<QAudioSink> _audio_sink;
    std::unique_ptr<WaveFile> _wave_file;
};

#endif // AUDIO_PLAYBACK_AUDIO_PLAYER_H