#include "SQLiteUtils.h"
#include "HBaseUtils.h"
#include "WaveSignalStruct.h"

int main() {
    // 1. 创建/打开 sqlite数据库
    SQLiteUtils sqlite_utils("wave.db");

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

    // 6. 初始化HBaseUtils
    HBaseUtils hbase_utils; //HBaseUtils是java中操作HBase的类名,HBaseUtils

//    hbase_utils.hello();
//    std::cout << "init ok" << std::endl;


    hbase_utils.create_table(sample_info.sample_name.c_str(), "WaveData_test"); // 第一个参数为HBase中的表名,第二个参数是列簇名

    std::cout << "create HBase table ok" << std::endl;

//     7. 将波形数据存入HBase
//    for (const auto &signal: signal_list) {
//        // smit应该在这里有一些解析数据帧等的代码
//        hbase_utils.put(sample_info.sample_name.c_str(), signal.signal_name, start_location, data);
//   }
    hbase_utils.put(sample_info.sample_name.c_str(), "testet","WaveData_test","testColumn",1111,"11111111111111111111");
    hbase_utils.put(sample_info.sample_name.c_str(), "testet","WaveData_test","testColumn",2222,"22222222222222222222");

    std::cout << "put ok" << std::endl;

    char* test_data = hbase_utils.get(sample_info.sample_name.c_str(), "testet","WaveData_test","testColumn",1111);
    std::cout << "get data: " << test_data << std::endl;

    char* test_data_1 = hbase_utils.get(sample_info.sample_name.c_str(), "testet","WaveData_test","testColumn",2222);
    std::cout << "get data: " << test_data_1 << std::endl;


    return 0;
}
