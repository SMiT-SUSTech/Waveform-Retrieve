#ifndef CPP_SQLITEUTILS_HZH_H
#define CPP_SQLITEUTILS_HZH_H

#include "../sqlite_20211019/sqlite_20211019/src/CppSQLite3.h"
#include "../cpp/WaveSignalStruct.h"

#include <algorithm>  // copy_n
#include <iostream>
// #include <string>  // not necessary, but I don't know why.
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
            this->msg = NULL;  // TODO
        }

        static const int NO_SAMPLE_INFO_RECORDS = 100;
        static const int SEVERAL_SAMPLE_INFO_RECORDS = 101;

        static const int SAMPLE_INFO_RECORD_ALREADY_EXISTS = 102;

        const char* errorMessage() {
            return this->msg;
        }
};


class WavePrj {

    private:  // FIXME  ok ?? ok. Public or private are both ok except the 5 APIs.
        char* db_name = nullptr;  // Each wave project corresponds to a SQLite ".db" file, and the filename will be the same as the project name.
        smit::sql::sqlite3_db* db;

    public:
        // WavePrj() = default;  // TODO  necessary ?? not necessary.

        WavePrj(const char* name) {

            const size_t name_length = strlen(name);
            this->db_name = new char[name_length + 1]{};
            std::copy_n(name , name_length , this->db_name);
            
            this->db = new smit::sql::sqlite3_db();
            if(this->db == nullptr) {
                std::cout << "Create m_pSqlDb failed!!!" << std::endl;
                exit(1);
            }

            // try {
                this->db->open(name);  // FIXME  no return code ?? Catch exceptions and exit.
            // } catch (smit::sql::sqlite3_exception& e) {
            //     std::cout << e.errorMessage() << std::endl;
            //     exit(1);
            // }
        }

        ~WavePrj() {
            std::cout << "Closing project [" << this->db_name << "]." << std::endl;
            this->db_name = nullptr;  // FIXME  really release memory ??
            // try {
                if(this->db != nullptr) this->db->close();
            // } catch (smit::sql::sqlite3_exception& e) {
            //     std::cout << e.errorMessage() << std::endl;
            //     exit(1);
            // }
        }



        /** SQLite Accessing Utils. */

        // sqlite3_db* open_db()  // TODO  necessary ?? not necessary.  // FIXME  static ??
        // void ddl_exec(const std::string &sql);  // "INSERT" are not suitable to be referred to as "data definition language".
        // void query_exec(const std::string &sql)qli

        bool is_table_exist(const char* k_table_name);  // FIXME  "does", not "is"? No, "is" is a naming standard.  // TODO  what does "k" mean ?? Google uses "k"+CamelCase to indicate a const param.  // TODO  become private ok ?? ok.
        void exec(const char* sql_format_str , ...);


        
        /** SMiT domain-specific operations. */

        void create_sampleinfo_table();
        void insert_sampleinfo(const std::string &sample_name , const long long &length);  // FIXME block_length not int ??
        SampleInfo select_sampleinfo(const std::string &sample_name);

        void create_wavesignal_table();
        void insert_wavesignal(const std::string &sample_name, const WaveSignal &wave_signal);
        void batch_insert_wavesignal(const std::string &sample_name, const std::vector<WaveSignal> &signal_list);
        // WaveSignal select_wavesignal(const std::string );
        std::vector<WaveSignal> select_wavesignal(const std::string &sample_name);

        // void get_signal_hbase_info(const char* signal_name);  // TODO  necessary ?? not implemented ?? Wait for discussions ...
        

};

class WaveUtils {

    private:
        static WavePrj* current_project;  // = NULL;
        static std::string* current_sample;  // = NULL;
        // FIXME  initial value ?? assign in cpp.

    public:
        /** SMiT required APIs */

        static int32_t SetPrj(std::string PrjName);  // SetPrjDB
        static int32_t SetSample(std::string SampleName , long long &block_length);  // SetWaveDB  // WaveName
        static int32_t StoreWaveSignal(std::vector<WaveSignal> &SignalList);

        static int32_t CheckSampleExists(std::string PrjName , std::string SampleName);  // GetWaveDB
        static int32_t GetWaveSignalList(std::vector<WaveSignal> &SignalList);

};

#endif //CPP_SQLITEUTILS_HZH_H