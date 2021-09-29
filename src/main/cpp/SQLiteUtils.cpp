#include "SQLiteUtils.h"

sqlite3* SQLiteUtils::open_table() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(table_name, &db);
    if (rc) {
        fprintf(stderr, "Cant't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    return db;
}

void SQLiteUtils::ddl_exec(const std::string &sql) {
    int rc;
    char *zErrMsg = nullptr;

    rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
}

void SQLiteUtils::create_sampleinfo_table() {
    const std::string sql = "CREATE TABLE  SampleInfo(" \
                            "SampleID      INT(32)           PRIMARY KEY AUTOINCREMENT  NOT NULL," \
                            "SampleName    VARCHAR(1024)                                NOU NULL," \
                            "BlockLength   INT(64) UNSIGNED                             NOT NULL);";
    SQLiteUtils::ddl_exec(sql);
}

void SQLiteUtils::create_wavesignal_table() {
    const std::string sql = "CREATE TABLE WaveSignal(" \
                            "SID         INT(64)           PRIMARY KEY AUTOINCREMENT  NOT NULL," \
                            "SampleID    INT(32)           PRIMARY KEY                NOT NULL," \
                            "SignalName  VARCHAR(8192)                                NOT NULL," \
                            "FID         INT(16) UNSIGNED                             NOT NULL," \
                            "Pos         INT(16) UNSIGNED                             NOT NULL);";
    SQLiteUtils::ddl_exec(sql);
}

void SQLiteUtils::insert_sampleinfo(const std::string &sample_name, const int &length) {
    std::ostringstream insert_sql;
    insert_sql << "INSERT INTO SampleInfo (SampleName, BlockLength) " \
                  "VALUES ('" << sample_name << "', " << length << ");";
    SQLiteUtils::ddl_exec(insert_sql.str());
}

SampleInfo SQLiteUtils::select_sampleinfo(const std::string &sample_name) {
    int rc = 0;
    int row_number = 0;
    int col_number = 0;
    char **azResult;
    char *zErrMsg = nullptr;

    std::ostringstream query;
    query << "SELECT * FROM SampleInfo WHERE SampleName='" << sample_name << "';";
    rc = sqlite3_get_table(db, query.str().c_str(), &azResult, &row_number, &col_number, &zErrMsg);
    SampleInfo result;
    if (rc == SQLITE_OK) {
        int index = col_number;
        for (int i=0; i<row_number; ++i) {
            for (int j=0; j<col_number; ++j) {
                if (strcmp(azResult[i], "SampleID") == 0) {
                    result.sample_id = atoi(azResult[index++]);
                } else if (strcmp(azResult[i], "SampleName") == 0) {
                    result.sample_name = azResult[index++];
                } else if (strcmp(azResult[i], "BlockLength") == 0) {
                    result.block_length = atoi(azResult[index++]);
                }
            }
        }
    }
    return result;
}

void SQLiteUtils::insert_wavesignal(const std::string &sample_name, const WaveSingal &wave_singal) {
    SampleInfo sample_info = SQLiteUtils::select_sampleinfo(sample_name) ;
    std::ostringstream insert_sql;
    insert_sql << "INSERT INTO WaveSignal (SampleID, SignalName, FID, Pos) " \
                  "VALUES (" << sample_info.sample_id << ", '" << sample_name << "', " << wave_singal.fid << \
                  ", " << wave_singal.pos << ");";
    SQLiteUtils::ddl_exec(insert_sql.str());
}

void SQLiteUtils::batch_insert_wavesignal(const std::string &sample_name, const std::vector<WaveSingal> &singal_list) {
    SampleInfo sample_info = SQLiteUtils::select_sampleinfo(sample_name);
    const char *insert_sql = "INSERT INTO WaveSignal (SampleID, SignalName, FID, Pos) VALUES(?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    const char *pzTail;

    sqlite3_prepare_v2(db, insert_sql, strlen(insert_sql), &stmt, &pzTail);

    int col = 1;
    for (const auto &singal: singal_list) {
        sqlite3_bind_int(stmt, col++, sample_info.sample_id);
        sqlite3_bind_text(stmt, col++, singal.singal_name, strlen(singal.singal_name), nullptr);
        sqlite3_bind_int(stmt, col++, singal.fid);
        sqlite3_bind_int(stmt, col++, singal.pos);

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
}

void SQLiteUtils::get_signal_hbase_info(const char *signal_name) {}
