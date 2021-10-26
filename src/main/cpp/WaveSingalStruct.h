#ifndef WAVEFORMRETRIEVE_WAVESINGALSTRUCT_H
#define WAVEFORMRETRIEVE_WAVESINGALSTRUCT_H

struct WaveSingal {
    char* singal_name;
    int8_t fid;
    uint16_t pos;
};

struct SampleInfo {
    int sample_id;
    std::string sample_name;
    long long block_length;
};

#endif //WAVEFORMRETRIEVE_WAVESINGALSTRUCT_H
