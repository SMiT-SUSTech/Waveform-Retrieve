#ifndef CPP_SQLITEUTILS_H
#define CPP_SQLITEUTILS_H

#include <sqlite3.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include "WaveSingalStruct.h"

class SQLiteUtils {
    char* table_name = nullptr;
    sqlite3 *db = nullptr;
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
    static int sqlite_callback(void *unused, int count, char **data, char **columns) {
        int idx;

        printf("There are %d column(s)\n", count);

        for (idx = 0; idx < count; idx++) {
            printf("The data in column \"%s\" is: %s\n", columns[idx], data[idx]);
        }

        printf("\n");

        return 0;
    }
    sqlite3* open_table();
    void create_wavesignal_table();
    void insert_wavaesignal(WaveSingal wave_singal);
    void get_signal_hbase_info(const char* signal_name);
};

#endif //CPP_SQLITEUTILS_H
