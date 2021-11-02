#ifndef CPP_SQLITEUTILS_H
#define CPP_SQLITEUTILS_H

#include "sqlite_src/CppSQLite3.h"
#include "../cpp/WaveSignalStruct.h"

#include <algorithm>
#include <iostream>
#include <vector>


class WaveSysException {

    public:
        int code;
        const char* msg;

        WaveSysException(int _code , const char* _msg) {
            this->code = _code;
            this->msg = _msg;
        }

        ~WaveSysException() {
            this->msg = nullptr;  // TODO
        }

        static const int NO_SAMPLE_INFO_RECORDS = 100;
        static const int SEVERAL_SAMPLE_INFO_RECORDS = 101;
        static const int SAMPLE_INFO_RECORD_ALREADY_EXISTS = 102;

        const char* errorMessage() {
            return this->msg;
        }
};


class WavePrj {
    private:
        char* db_name = nullptr;  // Each wave project corresponds to a SQLite ".db" file, and the filename will be the same as the project name.
        smit::sql::sqlite3_db* db;

    public:
        WavePrj(const char* name) {
            const size_t name_length = strlen(name);
            this->db_name = new char[name_length + 1]{};
            std::copy_n(name , name_length , this->db_name);
            
            this->db = new smit::sql::sqlite3_db();
            if(this->db == nullptr) {
                std::cout << "Create m_pSqlDb failed!!!" << std::endl;
                exit(1);
            }

            this->db->open(name);
        }

        ~WavePrj() {
            std::cout << "Closing project [" << this->db_name << "]." << std::endl;
            delete this->db_name;
            this->db_name = nullptr;
            if(this->db != nullptr) this->db->close();
            delete this->db;
        }

        /** SQLite Accessing Utils. */
        bool is_table_exist(const char* kTableName);
        void exec(const char* sql_format_str , ...);

        /** SMiT domain-specific operations. */
        void create_sampleinfo_table();
        void insert_sampleinfo(const std::string &kSampleName , const long long &kLength);
        SampleInfo select_sampleinfo(const std::string &sample_name);

        void create_wavesignal_table();
        void insert_wavesignal(const std::string &sample_name, const WaveSignal &wave_signal);
        void batch_insert_wavesignal(const std::string &sample_name, const std::vector<WaveSignal> &signal_list);
        std::vector<WaveSignal> select_wavesignal(const std::string &sample_name);
};

class WaveUtils {

    private:
        static WavePrj* current_project;
        static std::string current_sample;

    public:
        /** SMiT required APIs */
        static void getSampleName() {
            std::cout << "&&&&&&main sample name: " << current_sample << std::endl;
        }
        static int32_t SetPrj(std::string PrjName);  // SetPrjDB
        static int32_t SetSample(std::string SampleName , long long &block_length);  // SetWaveDB  // WaveName
        static int32_t StoreWaveSignal(std::vector<WaveSignal> &SignalList);

        static int32_t CheckSampleExists(std::string PrjName , std::string SampleName);  // GetWaveDB
        static int32_t GetWaveSignalList(std::vector<WaveSignal> &SignalList);

};

#endif //CPP_SQLITEUTILS_HZH_H