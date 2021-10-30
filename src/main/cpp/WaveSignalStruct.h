#ifndef WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H
#define WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H

struct WaveSignal {
    char* signal_name;
    int8_t fid;
    uint16_t pos;
};

struct SampleInfo {
    int sample_id;
    std::string sample_name;
    long long block_length;
};

#endif //WAVEFORMRETRIEVE_WAVESIGNALSTRUCT_H
