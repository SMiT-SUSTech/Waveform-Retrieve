#include "SQLiteUtils_HZH.h"

// FIXME  namespace legal ?? namespace necessary ??
// TODO  how to handle exceptions ?? Directly exit(1). And for normal case, returning the object (rather than its pointer) would also be ok.


/** SQLite Accessing Utils. */

// sqlite3_db* open_db()  // TODO  necessary ??  // FIXME  static ??

bool WavePrj::is_table_exist(const char* k_table_name) {  // FIXME  (const std::string &k_table_name) {
    return this->db->tableExists(k_table_name);
}

void WavePrj::exec(const char* sql_format_str , ...) {
    // try {
        char sql_buf[512] = {0};

        va_list p;
        va_start(p, sql_format_str);
        vsprintf(sql_buf, sql_format_str, p);
        va_end(p);

        std::cout << "Executing SQL: [" << sql_buf << "]" << std::endl;
        this->db->execDML(sql_buf);
        
    // } catch (smit::sql::sqlite3_exception& e) {
    //     std::cout << e.errorMessage() << std::endl;
    //     exit(1);
    //     return false;
    // }
    // return true;
}



/** SMiT domain-specific operations. */

const char* SAMPLE_INFO_TABLE_NAME = "SampleInfo";
const char* WAVE_SIGNAL_TABLE_NAME = "WaveSignal";


void WavePrj::create_sampleinfo_table() {
    if(is_table_exist(SAMPLE_INFO_TABLE_NAME)) return;  // not false
    exec("CREATE TABLE %s (\n" \
            "SampleID      INTEGER          PRIMARY KEY AUTOINCREMENT  NOT NULL,\n" \
            "SampleName    VARCHAR(1024)                               NOT NULL,\n" \
            "BlockLength   INT(64)                                     NOT NULL);\n",
            SAMPLE_INFO_TABLE_NAME);
}

void WavePrj::insert_sampleinfo(const std::string &sample_name , const long long &length) {
    // if() throw ...;  // TODO  if sample already exists.
    try{
        SampleInfo sample_info_obj = select_sampleinfo(sample_name);

        throw WaveSysException(WaveSysException::SAMPLE_INFO_RECORD_ALREADY_EXISTS , "insert_sampleinfo: Sample name already exists!");

    } catch (WaveSysException& e) {
        if(e.code != WaveSysException::NO_SAMPLE_INFO_RECORDS) throw e;
    }

    exec("INSERT INTO %s (SampleName, BlockLength) VALUES ('%s', %lld);" , SAMPLE_INFO_TABLE_NAME , sample_name.c_str() , length);
}

SampleInfo WavePrj::select_sampleinfo(const std::string &sample_name) {
    // try {
        char sql_buf[512] = {0};
        sprintf(sql_buf , "SELECT * FROM %s WHERE SampleName='%s';" , SAMPLE_INFO_TABLE_NAME , sample_name.c_str());
        std::cout << "Executing SQL: [" << sql_buf << "]" << std::endl;

        smit::sql::sqlite3_query result_1 = this->db->execQuery(sql_buf);

        if(result_1.eof()) {
            // std::cout <<  << std::endl;
            //exit(1);
            throw WaveSysException(WaveSysException::NO_SAMPLE_INFO_RECORDS , "select_sampleinfo: No records found!");
        }

        //while(! result_1.eof()) {
        SampleInfo sample_info_obj;
        sample_info_obj.sample_id    = result_1.getIntField("SampleID");
        sample_info_obj.sample_name  = result_1.getStringField("SampleName");  // FIXME  returns char*
        sample_info_obj.block_length = result_1.getInt64Field("BlockLength");  // FIXME  returns __int64.  // block_length max 46 bits.

        result_1.nextRow();
        //}
        if(! result_1.eof()) {
            // std::cout <<  << std::endl;
            // exit(1);
            throw WaveSysException(WaveSysException::SEVERAL_SAMPLE_INFO_RECORDS , "select_sampleinfo: More than one records found!");
        }

        return sample_info_obj;

        // result_1.finalize();  // not necessary, because invoked in sqlite3_query::~sqlite3_query()

    // } catch (smit::sql::sqlite3_exception& e) {
    //     std::cout << e.errorMessage() << std::endl;
    //     exit(1);
    // }
}



void WavePrj::create_wavesignal_table() {
    if(is_table_exist(WAVE_SIGNAL_TABLE_NAME)) return;  // not false
    exec("CREATE TABLE %s (\n" \
            "SID         INTEGER         PRIMARY KEY AUTOINCREMENT  NOT NULL,\n" \
            "SampleID    INT(32)                                    NOT NULL,\n" \
            "SignalName  VARCHAR(8192)                              NOT NULL,\n" \
            "FID         INT(16)                                    NOT NULL,\n" \
            "Pos         INT(16)                                    NOT NULL);\n",
            WAVE_SIGNAL_TABLE_NAME);
    // TODO  why SampleID unique ??  mistake, remove "UNIQUE".
    // TODO  SampleID INDEX ??
    exec("CREATE INDEX sample_id_index ON %s (SampleID);" , WAVE_SIGNAL_TABLE_NAME);  // TODO  need review.
}

void WavePrj::insert_wavesignal(const std::string &sample_name, const WaveSignal &wave_signal) {
    SampleInfo sample_info_obj = select_sampleinfo(sample_name);  // WavePrj::

    // TODO  judge whether signal exist ...

    exec("INSERT INTO WaveSignal (SampleID, SignalName, FID, Pos) VALUES (%d , '%s', %d , %d);" , 
            sample_info_obj.sample_id , wave_signal.signal_name , wave_signal.fid , wave_signal.pos);  // FIXME  the second param should be wave_signal.signal_name, rather than sample_name
}

void WavePrj::batch_insert_wavesignal(const std::string &sample_name, const std::vector<WaveSignal> &signal_list) {
    SampleInfo sample_info_obj = select_sampleinfo(sample_name);  // WavePrj::
    // try {
        // TODO  How to prepare statements when using smit-provided SQPLite ??
        smit::sql::sqlite3_statement compiled = this->db->compileStatement("INSERT INTO WaveSignal (SampleID, SignalName, FID, Pos) VALUES(?, ?, ?, ?);");

        for (const auto &signal: signal_list) {

            // TODO  judge whether signal exist ...

            compiled.bind(1 , sample_info_obj.sample_id);  // FIXME  sample_id passed in ?? No, it is obtained from sample_info_obj.
            compiled.bind(2 , signal.signal_name);
            compiled.bind(3 , signal.fid);
            compiled.bind(4 , signal.pos);

            compiled.execDML();
            compiled.reset();
        }

        compiled.finalize();

    // } catch (smit::sql::sqlite3_exception& e) {
    //     std::cout << e.errorMessage() << std::endl;
    //     exit(1);
    // }
}

std::vector<WaveSignal> WavePrj::select_wavesignal(const std::string &sample_name) {
    SampleInfo sample_info_obj = select_sampleinfo(sample_name);  // WavePrj::

    char sql_buf[512] = {0};
    sprintf(sql_buf , "SELECT * FROM %s WHERE SampleID=%d;" , WAVE_SIGNAL_TABLE_NAME , sample_info_obj.sample_id);
    std::cout << "Executing SQL: [" << sql_buf << "]" << std::endl;

    smit::sql::sqlite3_query result_1 = this->db->execQuery(sql_buf);

    std::vector<WaveSignal> v;
    while(! result_1.eof()) {
        WaveSignal wave_signal_obj;

        // wave_signal_obj.signal_name = result_1.getStringField("SignalName");  // FIXME  return type ??
        const char* signal_name_const = result_1.getStringField("SignalName");  // FIXME  return type ??
        const size_t signal_name_length = strlen(signal_name_const);
        wave_signal_obj.signal_name = new char[signal_name_length + 1]{};
        std::copy_n(signal_name_const , signal_name_length , wave_signal_obj.signal_name);

        wave_signal_obj.fid = result_1.getIntField("FID");  // FIXME  return type ??
        wave_signal_obj.pos = result_1.getIntField("Pos");  // FIXME  return type ??

        v.push_back(wave_signal_obj);

        result_1.nextRow();
    }

    return v;
}








/** SMiT required APIs */

#define SUCCESS_CODE 0
#define FAIL_CODE 1

WavePrj*  WaveUtils::current_project = NULL;
std::string*  WaveUtils::current_sample = NULL;  // FIXME  assign NULL to a non-pointer ?? Now change to pointer definition.

int32_t WaveUtils::SetPrj(std::string PrjName) {
    current_sample = NULL;

    try {
        if(current_project != NULL) current_project = NULL;  // TODO  check: should invoke ~WavePrj()

        current_project = new WavePrj((PrjName + ".db").c_str());
        current_project->create_sampleinfo_table();
        current_project->create_wavesignal_table();

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        current_project = NULL;
        return FAIL_CODE;
    }
    
    std::cout << "\nSwitched to project [" << PrjName << "]\n" << std::endl;
    return SUCCESS_CODE;
}

int32_t WaveUtils::SetSample(std::string SampleName , long long &block_length) {
    if(current_project == NULL) {
        std::cout << "Project not set." << std::endl;
        return FAIL_CODE;
    }

    try {
        current_sample = &SampleName;
        current_project->insert_sampleinfo(SampleName , block_length);  // TODO  what block length ?? an additional argument.

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        current_sample = NULL;
        return FAIL_CODE;
    } catch (WaveSysException& e) {
        // Catch the exception and do nothing here, because it's ok to have existing sample name.
    }

    std::cout << "\nSwitched to sample [" << SampleName << "]\n" << std::endl;
    return SUCCESS_CODE;
}

int32_t WaveUtils::StoreWaveSignal(std::vector<WaveSignal> &SignalList) {
    if(current_project == NULL) {
        std::cout << "Project not set." << std::endl;
        return FAIL_CODE;
    }
    if(current_sample == NULL) {
        std::cout << "Sample name not set." << std::endl;
        return FAIL_CODE;
    }

    try {
        current_project->batch_insert_wavesignal(*current_sample , SignalList);

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
    if(fp == NULL) return false;
    fclose(fp);
    return true;
}

int32_t WaveUtils::CheckSampleExists(std::string PrjName , std::string SampleName) {
    // why returning int32_t code ?? what does this function mean ?? Just to check the project + sample exists, and will not create new database (file) if not exists.

    const char* db_name = (PrjName + ".db").c_str();
    if(! check_file_exists(db_name)) {
        std::cout << "Project [" << PrjName << "] not found!" << std::endl;
        return FAIL_CODE;
    }

    try {
        WavePrj* prj2 = new WavePrj(db_name);
        prj2->select_sampleinfo(SampleName);  // if not found or found multiple records, it will throw WaveSysException.
        prj2 = NULL;

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;

    } catch (WaveSysException& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

int32_t WaveUtils::GetWaveSignalList(std::vector<WaveSignal> &SignalList) {
    if(current_project == NULL) {
        std::cout << "Project not set." << std::endl;
        return FAIL_CODE;
    }
    if(current_sample == NULL) {
        std::cout << "Sample name not set." << std::endl;
        return FAIL_CODE;
    }

    try {
        SignalList = current_project->select_wavesignal(*current_sample);  // TODO  Like this ??

    } catch (smit::sql::sqlite3_exception& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;

    } catch (WaveSysException& e) {
        std::cout << e.errorMessage() << std::endl;
        return FAIL_CODE;
    }

    return SUCCESS_CODE;
}

