#ifndef WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H
#define WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H

#include <string>

struct WaveSignal {
    char* signal_name;
    __int8_t fid;
    __uint16_t pos;
};

struct SampleInfo {
    int sample_id;
    std::string sample_name;
    long long block_length;
};

#endif //WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H
