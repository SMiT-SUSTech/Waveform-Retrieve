#ifndef WAVEFORMRETRIEVE_WAVEDATAUTILS_H
#define WAVEFORMRETRIEVE_WAVEDATAUTILS_H

int32_t StoreWaveData(std::string SampleName, std::vector<WaveSignalData> &WaveDatas);


int32_t GetWaveData(std::string SampleName, const long long kLengthBlock, std::vector<uint64_t> sig_ids, uint64_t start_pos, uint64_t req_count, uint32_t& act_count, std::vector<std::string>& wave_data);


#endif //WAVEFORMRETRIEVE_WAVEDATAUTILS_H
