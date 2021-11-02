//
// Created by Hobin on 2021/10/19.
//

#include "CppSQLite3.h"
#include <stdint.h>
#include <string>
#include <iostream>
using namespace smit::sql;
using namespace std;


static sqlite3_db *m_pSqlDb;
static sqlite3_statement m_SqlStatement;

static int32_t CreateTable(string TableName)
{
    if(!m_pSqlDb)
    {
        cout<<"m_pSqlDb is nullptr!!!"<<endl;
        return -1;
    }


    if(m_pSqlDb->tableExists(TableName.c_str()))
    {
        cout<<"Table "<<TableName<<" exist, delete it first."<<endl;

        try {
            char sql[512] = {0};
            sprintf(sql, "drop table %s;", TableName.c_str());
            m_pSqlDb->execDML(sql);
        } catch (sqlite3_exception& e)
        {
            cout<<e.errorMessage()<<endl;
            return -1;
        }
    }

    try {
        char sql[512] = {0};
        sprintf(sql, "create table %s("
                     "IndexId        INTEGER    NOT NULL    PRIMARY KEY    AUTOINCREMENT,"
                     "Signal        TEXT     UNIQUE,"
                     "SID            INT,"
                     "UNIQUE(Signal, SID)"
                     ");",
                TableName.c_str());
//            log_d("sql: %s", sql);
        m_pSqlDb->execDML(sql);

    } catch (sqlite3_exception& e)
    {
        cout<<e.errorMessage()<<endl;
        return -1;
    }

    return 0;
}

static int32_t StoreData(string TableName)
{
    if(!m_pSqlDb->tableExists(TableName.c_str()))
    {
        cout<<"Table "<<TableName<<" exist, please create it first!!"<<endl;
        return -1;
    }

    for(auto i=0; i<100; i++)
    {
        char sql[2048] = {0};
        string Signal = "test_signal" + to_string(i);
        try {
            sprintf(sql, "insert into %s(Signal, SID)"
                         " values('%s', %d)", TableName.c_str(), Signal.c_str(), i);
            m_pSqlDb->execDML(sql);
        }
        catch (sqlite3_exception& e)
        {
            cout<<e.errorMessage()<<endl;
            return -1;
        }
    }

    return 0;
}

static int32_t GetData(string TableName)
{
    if(!m_pSqlDb->tableExists(TableName.c_str()))
    {
        cout<<"Table "<<TableName<<" exist, please create it first!!"<<endl;
        return -1;
    }

    try {
        char sql[256] = {0};
        sprintf(sql,"select * from %s", TableName.c_str());
        sqlite3_query query = m_pSqlDb->execQuery(sql);

        cout<<"Data list:"<<endl;
        while(!query.eof())
        {
            string Signal = query.getStringField("Signal");
            uint32_t SID = query.getIntField("SID");
            cout<<"\t"<<Signal<<"\t"<<SID<<endl;
            query.nextRow();
        }

        query.finalize();
    }
    catch (sqlite3_exception& e)
    {
        cout<<e.errorMessage()<<endl;
        return -1;
    }

    return 0;
}


int main(int argc, char* argv[])
{
    printf("sqlite3 test...");

    m_pSqlDb = new sqlite3_db();
    if(m_pSqlDb == nullptr)
    {
        cout<<"Create m_pSqlDb failed!!!"<<endl;
        return -1;
    }

    const string DatabaseFile = "Database.db";
    m_pSqlDb->open(DatabaseFile.c_str());


    const string TableName = "test_example";
    int32_t Ret = CreateTable(TableName);
    if(Ret != 0)
    {
        cout<<"CreateTable failed!!!"<<endl;
        return -1;
    }

    Ret = StoreData(TableName);
    if(Ret != 0)
    {
        cout<<"StoreData failed!!!"<<endl;
        return -1;
    }

    Ret = GetData(TableName);
    if(Ret != 0)
    {
        cout<<"StoreData failed!!!"<<endl;
        return -1;
    }

    m_pSqlDb->close();

    return 0;
}