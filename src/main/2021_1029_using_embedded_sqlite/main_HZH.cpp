#include "SQLiteUtils_HZH.h"
#include "../cpp/WaveSignalStruct.h"


int main() {
    try{
        // 1. 创建/打开 sqlite数据库
        WavePrj sqlite_utils("wave.db");

        // 2. 创建两张表
        //    (1). 采样信息表
        //    (2). 波形信号列表表
        sqlite_utils.create_sampleinfo_table();
        sqlite_utils.create_wavesignal_table();

        // 3. 初始化,采样信息
        SampleInfo sample_info;
        sample_info.sample_name = "first_sample_test"; // 采样信息标识
        sample_info.block_length = 65536; // 在HBase中存储数据块的长度

        // 4. 将 采样信息 写入sqlite
        sqlite_utils.insert_sampleinfo(sample_info.sample_name, sample_info.block_length);

        // 5. 将 信号列表信息 写入sqlite
        std::vector<WaveSignal> signal_list;
        sqlite_utils.batch_insert_wavesignal(sample_info.sample_name, signal_list);

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        exit(1);
    } catch (WaveSysException& e) {
        std::cout << e.errorMessage() << std::endl;
        exit(1);
    }

    // some later operations ...

    return 0;
}