////////////////////////////////////////////////////////////////////////////////
// CppSQLite3 - A C++ wrapper around the SQLite3 embedded database library.
//
// Copyright (c) 2004..2007 Rob Groves. All Rights Reserved. rob.groves@btinternet.com
// 
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without a written
// agreement, is hereby granted, provided that the above copyright notice, 
// this paragraph and the following two paragraphs appear in all copies, 
// modifications, and distributions.
//
// IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
// PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". THE AUTHOR HAS NO OBLIGATION
// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
// V3.0		03/08/2004	-Initial Version for sqlite3
//
// V3.1		16/09/2004	-Implemented getXXXXField using sqlite3 functions
//						-Added CppSQLiteDB3::tableExists()
//
// V3.2		01/07/2005	-Fixed execScalar to handle a NULL result
//			12/07/2007	-Added CppSQLiteDB::IsAutoCommitOn()
//						-Added int64 functions to CppSQLite3Query
//						-Added Name based parameter binding to CppSQLite3Statement.
////////////////////////////////////////////////////////////////////////////////
#ifndef _CppSQLite3_H_
#define _CppSQLite3_H_

#include "sqlite3.h"
#include <cstdio>
#include <cstring>

#define CPPSQLITE_ERROR 1000

namespace smit {
    namespace sql {
        class sqlite3_exception {
        public:

            sqlite3_exception(const int nErrCode,
                              char *szErrMess,
                              bool bDeleteMsg = true);

            sqlite3_exception(const sqlite3_exception &e);

            virtual ~sqlite3_exception();

            const int errorCode() { return mnErrCode; }

            const char *errorMessage() { return mpszErrMess; }

            static const char *errorCodeAsString(int nErrCode);

        private:

            int mnErrCode;
            char *mpszErrMess;
        };


        class sqlite3_buffer {
        public:

            sqlite3_buffer();

            ~sqlite3_buffer();

            const char *format(const char *szFormat, ...);

            operator const char *() { return mpBuf; }

            void clear();

        private:

            char *mpBuf;
        };


        class sqlite3_binary {
        public:

            sqlite3_binary();

            ~sqlite3_binary();

            void setBinary(const unsigned char *pBuf, int nLen);

            void setEncoded(const unsigned char *pBuf);

            const unsigned char *getEncoded();

            const unsigned char *getBinary();

            int getBinaryLength();

            unsigned char *allocBuffer(int nLen);

            void clear();

        private:

            unsigned char *mpBuf;
            int mnBinaryLen;
            int mnBufferLen;
            int mnEncodedLen;
            bool mbEncoded;
        };


        class sqlite3_query {
        public:

            sqlite3_query();

            sqlite3_query(const sqlite3_query &rQuery);

            sqlite3_query(sqlite3 *pDB,
                          sqlite3_stmt *pVM,
                          bool bEof,
                          bool bOwnVM = true);

            sqlite3_query &operator=(const sqlite3_query &rQuery);

            virtual ~sqlite3_query();

            int numFields();

            int fieldIndex(const char *szField);

            const char *fieldName(int nCol);

            const char *fieldDeclType(int nCol);

            int fieldDataType(int nCol);

            const char *fieldValue(int nField);

            const char *fieldValue(const char *szField);

            int getIntField(int nField, int nNullValue = 0);

            int getIntField(const char *szField, int nNullValue = 0);

            sqlite_int64 getInt64Field(int nField, sqlite_int64 nNullValue = 0);

            sqlite_int64 getInt64Field(const char *szField, sqlite_int64 nNullValue = 0);

            double getFloatField(int nField, double fNullValue = 0.0);

            double getFloatField(const char *szField, double fNullValue = 0.0);

            const char *getStringField(int nField, const char *szNullValue = "");

            const char *getStringField(const char *szField, const char *szNullValue = "");

            const unsigned char *getBlobField(int nField, int &nLen);

            const unsigned char *getBlobField(const char *szField, int &nLen);

            bool fieldIsNull(int nField);

            bool fieldIsNull(const char *szField);

            bool eof();

            void nextRow();

            void finalize();

        private:

            void checkVM();

            sqlite3 *mpDB;
            sqlite3_stmt *mpVM;
            bool mbEof;
            int mnCols;
            bool mbOwnVM;
        };


        class sqlite3_table {
        public:

            sqlite3_table();

            sqlite3_table(const sqlite3_table &rTable);

            sqlite3_table(char **paszResults, int nRows, int nCols);

            virtual ~sqlite3_table();

            sqlite3_table &operator=(const sqlite3_table &rTable);

            int numFields();

            int numRows();

            const char *fieldName(int nCol);

            const char *fieldValue(int nField);

            const char *fieldValue(const char *szField);

            int getIntField(int nField, int nNullValue = 0);

            int getIntField(const char *szField, int nNullValue = 0);

            double getFloatField(int nField, double fNullValue = 0.0);

            double getFloatField(const char *szField, double fNullValue = 0.0);

            const char *getStringField(int nField, const char *szNullValue = "");

            const char *getStringField(const char *szField, const char *szNullValue = "");

            bool fieldIsNull(int nField);

            bool fieldIsNull(const char *szField);

            void setRow(int nRow);

            void finalize();

        private:

            void checkResults();

            int mnCols;
            int mnRows;
            int mnCurrentRow;
            char **mpaszResults;
        };


        class sqlite3_statement {
        public:

            sqlite3_statement();

            sqlite3_statement(const sqlite3_statement &rStatement);

            sqlite3_statement(sqlite3 *pDB, sqlite3_stmt *pVM);

            virtual ~sqlite3_statement();

            sqlite3_statement &operator=(const sqlite3_statement &rStatement);

            int execDML();

            sqlite3_query execQuery();

            void bind(int nParam, const sqlite_int64 int64value);

            void bind(int nParam, const char *szValue);

            void bind(int nParam, const int nValue);

            void bind(int nParam, const double dwValue);

            void bind(int nParam, const unsigned char *blobValue, int nLen);

            void bindNull(int nParam);

            int bindParameterIndex(const char *szParam);

            void bind(const char *szParam, const sqlite_int64 int64value);

            void bind(const char *szParam, const char *szValue);

            void bind(const char *szParam, const int nValue);

            void bind(const char *szParam, const double dwValue);

            void bind(const char *szParam, const unsigned char *blobValue, int nLen);

            void bindNull(const char *szParam);

            void reset();

            void finalize();

        private:

            void checkDB();

            void checkVM();

            sqlite3 *mpDB;
            sqlite3_stmt *mpVM;
        };


        class sqlite3_db {
        public:

            sqlite3_db();

            virtual ~sqlite3_db();

            void open(const char *szFile);

            void close();

            bool tableExists(const char *szTable);

            int execDML(const char *szSQL);

            sqlite3_query execQuery(const char *szSQL);

            int execScalar(const char *szSQL, int nNullValue = 0);

            sqlite3_table getTable(const char *szSQL);

            sqlite3_statement compileStatement(const char *szSQL);

            sqlite_int64 lastRowId();

            void interrupt() { sqlite3_interrupt(mpDB); }

            void setBusyTimeout(int nMillisecs);

            static const char *SQLiteVersion() { return SQLITE_VERSION; }

            static const char *SQLiteHeaderVersion() { return SQLITE_VERSION; }

            static const char *SQLiteLibraryVersion() { return sqlite3_libversion(); }

            static int SQLiteLibraryVersionNumber() { return sqlite3_libversion_number(); }

            bool IsAutoCommitOn();

        private:

            sqlite3_db(const sqlite3_db &db);

            sqlite3_db &operator=(const sqlite3_db &db);

            sqlite3_stmt *compile(const char *szSQL);

            void checkDB();

            sqlite3 *mpDB;
            int mnBusyTimeoutMs;
        };
    }
}
#endif
