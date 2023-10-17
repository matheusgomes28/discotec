#include <audio_playback/audio_device.h>

#include <QAudioFormat>


// namespace
// {
//     void delete_q_audio_device(QAudioDeviceInfo* prt)
//     {
//         if (ptr)
//         {
//             delete ptr;
//         }
//     }
// } // namespace


// AudioDevice::AudioDevice(AudioFormat const& format)
//     : _device_name{"unset"}, _audio_format{format}
// {
//     // Create the audio format
//     QAudioFormat q_audio_format;
//     q_audio_format.setSampleRate(format.sample_rate);
//     q_audio_format.setChannels(format.n_channels);
//     q_audio_format.setSampleSize(format.sample_size);
//     q_audio_format.setSampleType(QAudioFormat::SignedInt);
//     q_audio_format.setByteOrder(QAudioFormat::LittleEndian);
//     q_audio_format.setCodec("audio/pcm");

//     // This where I can get all the Device info
//     // from QT
//     _q_audio_device = {new QAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice()), delete_q_audio_device};
    

//     // Set the format to the nearest    
//     if (!_q_audio_device->isFormatSupported(q_audio_format))
//     {
//         auto const nearest_format = _q_audio_device->nearestFormat(a_audio_format);
//         // Reset all of the stuff in `format`
//         format.codec = nearest_format.codec();
//         format.n_channels = nearest_format.channelCount();
//         format.sample_rate = nearest_format.sampleRate();
//         format.sample_size = nearest_format.sampleSize();
//     }
// }