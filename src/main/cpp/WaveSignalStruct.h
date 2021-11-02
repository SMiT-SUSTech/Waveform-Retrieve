#ifndef WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H
#define WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H

#include <string>
#include <vector>

struct WaveSignalInfo {
    std::string signal_name;
    __int8_t fid;
    __uint16_t pos;
};

struct SampleInfo {
    int sample_id;
    std::string sample_name;
    long long block_length;
};

struct WaveSignalData {
    uint64_t sid;
    uint64_t start_local;
    std::string data;
};

#endif //WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H
