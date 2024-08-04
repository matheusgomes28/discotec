#ifndef AUDIO_PLAYBACK_AUDIO_FILE_H
#define AUDIO_PLAYBACK_AUDIO_FILE_H

#include <QAudioBuffer>
#include <QAudioFormat>

#include <memory>
#include <optional>

template <typename T>
class AudioFile;

class WaveFile
{
public:
    /// @brief Create a WaveFile from a file in the filesystem
    /// @param path is a valid path to a wave file
    WaveFile(std::string const& path);

    /// @brief the destructor baby
    ~WaveFile();

    /// @brief Gets the QAudioFormat corresponding to the format
    /// of the loaded audio file data 
    /// @return the QAudioFormat with the same properties as the
    /// loaded file, nullopt otherwise
    std::optional<QAudioFormat> get_format() const;

    /// @brief This function will return the audio format inside the
    /// requested audio file
    /// @return The QAudioBuffer containing the data in the file, or nullopt
    /// otherwise
    std::optional<QAudioBuffer> get_data() const;

private:
    std::unique_ptr<AudioFile<std::int32_t>> _audio_file;
};

#endif // AUDIO_PLAYBACK_AUDIO_FILE_H