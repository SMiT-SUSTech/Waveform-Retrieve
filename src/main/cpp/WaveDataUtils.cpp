#include "WaveSignalStruct.h"
#include "HBaseUtils.h"
#include <math.h>

int32_t StoreWaveData(std::string SampleName, std::vector<WaveSignalData> &WaveDatas) {
    HBaseUtils hbase_utils;
    hbase_utils.create_table(SampleName.c_str());
    for (const auto& wavedata: WaveDatas) {
        hbase_utils.put(SampleName.c_str(), std::to_string(wavedata.sid).c_str(), wavedata.start_local, wavedata.data);
    }
    return 0;
}

int32_t GetWaveData(std::string SampleName, const long long kLengthBlock, std::vector<uint64_t> sig_ids, uint64_t start_pos, uint64_t req_count, uint32_t& act_count, std::vector<std::string>& wave_data) {
    HBaseUtils hbase_utils;

    long long start_location = (start_pos / kLengthBlock) * kLengthBlock;
    long long block_number = ceil(double(req_count-start_pos)/kLengthBlock);
    act_count = block_number * kLengthBlock;
    for (const auto& sid: sig_ids) {
        for (long long i=0; i<block_number; ++i) {
            std::string data(hbase_utils.get(SampleName.c_str(), std::to_string(sid).c_str(), start_location+i));
            wave_data.push_back(data);
        }
    }
    return 0;
}
