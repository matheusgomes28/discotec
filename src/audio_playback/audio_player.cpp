#include <audio_playback/audio_player.h>
#include <audio_playback/audio_file.h>

#include <QAudioSink>


AudioPlayer::AudioPlayer()
{
    // The sink and audio files are
    // null here
}

auto AudioPlayer::load_wave(std::string const& filename) -> bool
{
    _wave_file = std::make_unique<WaveFile>(filename);

    auto const format = _wave_file->get_format();
    if (!format)
    {
        _wave_file.release();
        return false;
    }

    // This is cursed
    _audio_sink = std::make_unique<QAudioSink>(format.value(), nullptr);
    return true;
}
