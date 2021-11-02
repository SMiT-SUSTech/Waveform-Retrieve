#ifndef CPP_SQLITEUTILS_H
#define CPP_SQLITEUTILS_H

#include <sqlite3.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "WaveSignalStruct.h"

class SQLiteUtils {
    char* table_name = nullptr;
    sqlite3 *db = nullptr;
    void ddl_exec(const std::string &sql);
public:
    SQLiteUtils() = default;
    SQLiteUtils(const char* name) {
        const size_t name_length = strlen(name);
        table_name = new char[name_length + 1]{};
        std::copy_n(name, name_length, table_name);
        int rc;

        rc = sqlite3_open(table_name, &db);
        if (rc) {
            fprintf(stderr, "Cant't open database: %s\n", sqlite3_errmsg(db));
            exit(0);
        } else {
            fprintf(stdout, "Opened database successfully\n");
        }
    }
    ~SQLiteUtils() {
        table_name = nullptr;
        if (db != nullptr) {
            sqlite3_close(db);
        }
    }

    sqlite3* open_table();
//    void query_exec(const std::string &sql)qli
    bool is_table_exist(const std::string &kTableName);
    void create_sampleinfo_table();
    void create_wavesignal_table();
    void insert_sampleinfo(const std::string &sample_name, const int &length);
    SampleInfo select_sampleinfo(const std::string &sample_name);
    void insert_wavesignal(const std::string &sample_name, const WaveSignal &wave_signal);
    void batch_insert_wavesignal(const std::string &sample_name, const std::vector<WaveSignal> &signal_list);
//    WaveSignal select_wavesignal(const std::string );
    void get_signal_hbase_info(const char* signal_name);
};

#endif //CPP_SQLITEUTILS_H
