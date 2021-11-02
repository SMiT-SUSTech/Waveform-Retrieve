#include "SQLiteUtils.h"
#include "WaveSignalStruct.h"

int main() {
    try{


        std::cout << "Warning: this demo does not judge the return codes of the 5 APIs!"  << std::endl;


        std::cout << "\n\n===================================" << std::endl;
        std::cout << "WaveUtils::SetPrj" << std::endl;
        WaveUtils::SetPrj("proj_2021_1101_1159");

        std::cout << "\n\n===================================" << std::endl;
        std::cout << "WaveUtils::SetSample" << std::endl;
        long long block_length = 1234567654321;
        WaveUtils::SetSample("sample_1" , block_length);

        std::cout << "\n\n===================================" << std::endl;
        std::cout << "WaveUtils::StoreWaveSignal" << std::endl;
        std::vector<WaveSignalInfo> signal_list;
        signal_list.push_back({"led_0" , 1 , 10});
        signal_list.push_back({"led_1" , 1 , 20});
        signal_list.push_back({"led_2" , 1 , 30});
        signal_list.push_back({"led_3" , 1 , 40});
        signal_list.push_back({"button_0" , 1 , 50});
        signal_list.push_back({"button_1" , 1 , 60});
//        std::cout << "main: " << WaveUtils
        WaveUtils::getSampleName();
        WaveUtils::StoreWaveSignal(signal_list);

        std::cout << "\n\n===================================" << std::endl;
        std::cout << "WaveUtils::GetWaveSignalList" << std::endl;
        std::vector<WaveSignalInfo> signal_list_2;
        WaveUtils::GetWaveSignalList(signal_list_2);
        for(int i=0 ; i<signal_list_2.size() ; ++i) {
            WaveSignalInfo sig = signal_list_2[i];
            std::cout << "signal_name=[" << sig.signal_name << "] , fid=" << int(sig.fid) << " , pos=" << sig.pos << std::endl;
            // Note: sig.fid is of __int_8 type, and cannot be printed directly. So it should be cast to int().
        }


        std::cout << "\n\n===================================" << std::endl;
        std::cout << "WaveUtils::CheckSampleExists" << std::endl;
        WaveUtils::CheckSampleExists("not_existing_proj" , "sample_1");

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        exit(1);
    } catch (WaveSysException& e) {
        std::cout << e.errorMessage() << std::endl;
        exit(1);
    }

    return 0;
}