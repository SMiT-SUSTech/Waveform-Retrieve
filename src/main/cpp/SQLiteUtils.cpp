#include "SQLiteUtils.h"

/** SQLite Accessing Utils. */

bool WavePrj::is_table_exist(const char* k_table_name) {
    return this->db->tableExists(k_table_name);
}

void WavePrj::exec(const char* sql_format_str , ...) {
    char sql_buf[8192] = {0};

    va_list p;
    va_start(p, sql_format_str);
    vsprintf(sql_buf, sql_format_str, p);
    va_end(p);

    std::cout << "Executing SQL: [" << sql_buf << "]" << std::endl;
    this->db->execDML(sql_buf);
}

/** SMiT domain-specific operations. */

const char* SAMPLE_INFO_TABLE_NAME = "SampleInfo";
const char* WAVE_SIGNAL_TABLE_NAME = "WaveSignalInfo";

void WavePrj::create_sampleinfo_table() {
    if(is_table_exist(SAMPLE_INFO_TABLE_NAME)) return;
    exec("CREATE TABLE %s (\n" \
            "SampleID      INTEGER          PRIMARY KEY AUTOINCREMENT  NOT NULL,\n" \
            "SampleName    VARCHAR(1024)    UNIQUE                     NOT NULL,\n" \
            "BlockLength   INT(64)                                     NOT NULL);\n",
            SAMPLE_INFO_TABLE_NAME);
}

void WavePrj::insert_sampleinfo(const std::string &sample_name , const long long &length) {
    try{
        SampleInfo sample_info_obj = select_sampleinfo(sample_name);

        throw WaveSysException(WaveSysException::SAMPLE_INFO_RECORD_ALREADY_EXISTS , "insert_sampleinfo: Sample name already exists!");
    } catch (WaveSysException& e) {
        if(e.code != WaveSysException::NO_SAMPLE_INFO_RECORDS) throw e;
    }

    exec("INSERT INTO %s (SampleName, BlockLength) VALUES ('%s', %lld);" , SAMPLE_INFO_TABLE_NAME , sample_name.c_str() , length);
}

SampleInfo WavePrj::select_sampleinfo(const std::string &sample_name) {
    char sql_buf[8192] = {0};
    sprintf(sql_buf , "SELECT * FROM %s WHERE SampleName='%s';" , SAMPLE_INFO_TABLE_NAME , sample_name.c_str());
    std::cout << "Executing SQL: [" << sql_buf << "]" << std::endl;

    smit::sql::sqlite3_query result_1 = this->db->execQuery(sql_buf);

    if(result_1.eof()) {
        throw WaveSysException(WaveSysException::NO_SAMPLE_INFO_RECORDS , "select_sampleinfo: No records found!");
    }

    SampleInfo sample_info_obj;
    sample_info_obj.sample_id    = result_1.getIntField("SampleID");
    sample_info_obj.sample_name  = result_1.getStringField("SampleName");  // FIXME  returns char*
    sample_info_obj.block_length = result_1.getInt64Field("BlockLength");  // FIXME  returns __int64.  // block_length max 46 bits.

    result_1.nextRow();

    if(! result_1.eof()) {
        throw WaveSysException(WaveSysException::SEVERAL_SAMPLE_INFO_RECORDS , "select_sampleinfo: More than one records found!");
    }

    return sample_info_obj;
}

void WavePrj::create_wavesignal_table() {
    if(is_table_exist(WAVE_SIGNAL_TABLE_NAME)) return;
    exec("CREATE TABLE %s (\n" \
            "SID         INTEGER         PRIMARY KEY AUTOINCREMENT  NOT NULL,\n" \
            "SampleID    INT(32)                                    NOT NULL,\n" \
            "SignalName  VARCHAR(8192)   UNIQUE                     NOT NULL,\n" \
            "FID         INT(16)                                    NOT NULL,\n" \
            "Pos         INT(16)                                    NOT NULL);\n",
            WAVE_SIGNAL_TABLE_NAME);

    exec("CREATE INDEX sample_id_index ON %s (SampleID);" , WAVE_SIGNAL_TABLE_NAME);
}

void WavePrj::insert_wavesignal(const std::string &sample_name, const WaveSignalInfo &wave_signal) {
    SampleInfo sample_info_obj = select_sampleinfo(sample_name);

    // TODO  judge whether signal exist ...

    exec("INSERT INTO WaveSignalInfo (SampleID, SignalName, FID, Pos) VALUES (%d , '%s', %d , %d);" ,
            sample_info_obj.sample_id , wave_signal.signal_name.c_str(), wave_signal.fid , wave_signal.pos);  // FIXME  the second param should be wave_signal.signal_name, rather than sample_name
}

void WavePrj::batch_insert_wavesignal(const std::string &sample_name, const std::vector<WaveSignalInfo> &signal_list) {
    SampleInfo sample_info_obj = select_sampleinfo(sample_name);
        smit::sql::sqlite3_statement compiled = this->db->compileStatement("INSERT INTO WaveSignalInfo (SampleID, SignalName, FID, Pos) VALUES(?, ?, ?, ?);");

        for (const auto &signal: signal_list) {
            compiled.bind(1 , sample_info_obj.sample_id);
            compiled.bind(2 , signal.signal_name.c_str());
            compiled.bind(3 , signal.fid);
            compiled.bind(4 , signal.pos);

            compiled.execDML();
            compiled.reset();
        }

        compiled.finalize();
}

std::vector<WaveSignalInfo> WavePrj::select_wavesignal(const std::string &sample_name) {
    SampleInfo sample_info_obj = select_sampleinfo(sample_name);

    char sql_buf[8192] = {0};
    sprintf(sql_buf , "SELECT * FROM %s WHERE SampleID=%d;" , WAVE_SIGNAL_TABLE_NAME , sample_info_obj.sample_id);
    std::cout << "Executing SQL: [" << sql_buf << "]" << std::endl;

    smit::sql::sqlite3_query result_1 = this->db->execQuery(sql_buf);

    std::vector<WaveSignalInfo> v;
    while(! result_1.eof()) {
        WaveSignalInfo wave_signal_obj;

        wave_signal_obj.signal_name = result_1.getStringField("SignalName");

        wave_signal_obj.fid = result_1.getIntField("FID");
        wave_signal_obj.pos = result_1.getIntField("Pos");

        v.push_back(wave_signal_obj);

        result_1.nextRow();
    }

    return v;
}


/** SMiT required APIs */

#define SUCCESS_CODE 0
#define FAIL_CODE 1

WavePrj*  WaveUtils::current_project = nullptr;
std::string  WaveUtils::current_sample = "";  // FIXME  assign NULL to a non-pointer ?? Now change to pointer definition.

int32_t WaveUtils::SetPrj(std::string PrjName) {
    current_sample = "";

    try {
        if(current_project != nullptr) current_project = nullptr;  // TODO  check: should invoke ~WavePrj()

        current_project = new WavePrj((PrjName + ".db").c_str());
        current_project->create_sampleinfo_table();
        current_project->create_wavesignal_table();

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        current_project = nullptr;
        return FAIL_CODE;
    }
    
    std::cout << "\nSwitched to project [" << PrjName << "]\n" << std::endl;
    return SUCCESS_CODE;
}

int32_t WaveUtils::SetSample(std::string SampleName , long long &block_length) {
    if(current_project == nullptr) {
        std::cout << "Project not set." << std::endl;
        return FAIL_CODE;
    }

    try {
        current_sample = SampleName;
        current_project->insert_sampleinfo(SampleName , block_length);

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        current_sample = "";
        return FAIL_CODE;
    } catch (WaveSysException& e) {
        // Catch the exception and do nothing here, because it's ok to have existing sample name.
    }

    std::cout << "\nSwitched to sample [" << SampleName << "]\n" << std::endl;
    return SUCCESS_CODE;
}

int32_t WaveUtils::StoreWaveSignal(std::vector<WaveSignalInfo> &SignalList) {
    if(current_project == nullptr) {
        std::cout << "Project not set." << std::endl;
        return FAIL_CODE;
    }
    if(current_sample == "") {
        std::cout << "Sample name not set." << std::endl;
        return FAIL_CODE;
    }

    try {
        current_project->batch_insert_wavesignal(current_sample , SignalList);

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;

    } catch (WaveSysException& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

inline bool check_file_exists(const char* path) {  // TODO  check
    FILE* fp = fopen(path , "r");
    if(fp == nullptr) return false;
    fclose(fp);
    return true;
}

int32_t WaveUtils::CheckSampleExists(std::string PrjName , std::string SampleName) {
    const char* db_name = (PrjName + ".db").c_str();
    if(! check_file_exists(db_name)) {
        std::cout << "Project [" << PrjName << "] not found!" << std::endl;
        return FAIL_CODE;
    }

    try {
        WavePrj* prj2 = new WavePrj(db_name);
        prj2->select_sampleinfo(SampleName);  // if not found or found multiple records, it will throw WaveSysException.
        prj2 = nullptr;

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;

    } catch (WaveSysException& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

int32_t WaveUtils::GetWaveSignalList(std::vector<WaveSignalInfo> &SignalList) {
    if(current_project == nullptr) {
        std::cout << "Project not set." << std::endl;
        return FAIL_CODE;
    }
    if(current_sample == "") {
        std::cout << "Sample name not set." << std::endl;
        return FAIL_CODE;
    }

    try {
        SignalList = current_project->select_wavesignal(current_sample);

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;

    } catch (WaveSysException& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

