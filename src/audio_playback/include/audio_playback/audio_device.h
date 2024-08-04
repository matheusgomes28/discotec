#ifndef AUDIO_PLAYBACK_AUDIO_DEVICE_H
#define AUDIO_PLAYBACK_AUDIO_DEVICE_H

#include <memory>
#include <string>

// BEGIN_QT_NAMESPACE
// class QAudioDeviceInfo;
// QT_END_NAMESPACE

// using QAudioDeviceInfoDeleter = void(*)(QAudioDeviceInfo*);
// using QAudioDeviceInfoPtr = std::unique_ptr<QAudioDeviceInfo, QAudioDeviceInfoDeleter>;

// // TODO : sample type = SignedInt
// // TODO : byte order = LittleEndian
// struct AudioFormat
// {
//     int sample_rate;
//     int n_channels;
//     int sample_size;
//     std::string codec;
// };

// class AudioDevice
// {
//     explicit AudioDevice(AudioFormat const& format);

//     std::string get_name() const;

// private:
//     std::string _device_name;
//     AudioFormat _audio_format;

//     QAudioDeviceInfoPtr _q_audio_device;
// };

#endif // AUDIO_PLAYBACK_AUDIO_DEVICE_H
