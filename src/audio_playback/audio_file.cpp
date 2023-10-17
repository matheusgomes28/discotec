#include <audio_playback/audio_file.h>

#include <AudioFile.h>
#include <QAudioBuffer>
#include <QAudioFormat>

#include <optional>

WaveFile::WaveFile(std::string const& path)
    : _audio_file{new AudioFile<float>{}}
{
    // TODO : In the future we want to drop this
    // _audio_file because it duplicates the audio
    // Load the data
    if (_audio_file) {
        _audio_file->load(path);
    }
}

WaveFile::~WaveFile()
{
}

auto WaveFile::get_format() const -> std::optional<QAudioFormat>
{
    if (!_audio_file)
    {
        std::nullopt;
    }

    // Create the audio format
    QAudioFormat audio_format;
    audio_format.setSampleRate(_audio_file->getSampleRate());
    audio_format.setChannelCount(_audio_file->getNumChannels());
    audio_format.setSampleRate(_audio_file->getSampleRate());
    audio_format.setSampleFormat(QAudioFormat::Float);
    
    // TODO : What happens if the file doesn't have only 2 channels?
    audio_format.setChannelConfig(QAudioFormat::ChannelConfigStereo);

    return audio_format;
}

auto WaveFile::get_data() const -> std::optional<QAudioBuffer>
{
    if (!_audio_file)
    {
        std::nullopt;
    }
    
    auto const format = get_format();
    if (!format)
    {
        return std::nullopt;
    }


    // TODO : Interleave the data here
    auto const num_frames = _audio_file->getNumSamplesPerChannel();
    QAudioBuffer buffer{num_frames * 2, format.value(), -1};

    for (int i = 0; i < num_frames; ++i)
    {
        // TODO : Does this automatically convert the float
        // to qreal doubles?
        auto data = buffer.data<float>();
        data[i * 2] = _audio_file->samples[0][i];
        data[i * 2 + 1] = _audio_file->samples[1][i];
    }

    return buffer;
}
