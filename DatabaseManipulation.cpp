#include "DatabaseManipulation.h"


// Global variables for database handles
SQLHENV henv;
SQLHDBC hdbc;
SQLHSTMT hstmt;

void initializeDatabase() {
    SQLRETURN retcode;

    // Allocate environment handle
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

    // Set the ODBC version environment attribute
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    // Allocate connection handle
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

    // Connection string
    SQLWCHAR connStr[] = L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=project6group5-server.database.windows.net,1433;DATABASE=Project6Group5DB;UID=group5Guest;PWD=gr0up5Gu3st";

    // Connect to the database
    SQLSMALLINT outConnStrLength;
    retcode = SQLDriverConnect(hdbc, NULL, connStr, SQL_NTS, NULL, 0, &outConnStrLength, SQL_DRIVER_NOPROMPT);

    // Allocate statement handle
    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
}

int getHighScore(const std::string& username) {
    SQLRETURN retcode;
    SQLINTEGER highScore = 0;

    std::wstring query = L"SELECT HighScore FROM UserInfo WHERE Username = '" + std::wstring(username.begin(), username.end()) + L"'";
    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_SLONG, &highScore, 0, NULL);
    }
    else {
        throw std::runtime_error("User not found or no HighScore available for '" + username + "'");
    }

    return highScore;
}


std::string getPassword(const std::string& username) {
    SQLRETURN retcode;
    SQLCHAR dbPassword[100]; // Adjust size as needed

    std::wstring query = L"SELECT Password FROM UserInfo WHERE Username = '" + std::wstring(username.begin(), username.end()) + L"'";
    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_CHAR, dbPassword, sizeof(dbPassword), NULL);
    }
    else {
        throw std::runtime_error("User not found or no Password available for '" + username + "'");
    }

    return std::string(reinterpret_cast<char*>(dbPassword));
}


std::string getEmail(const std::string& username) {
    SQLRETURN retcode;
    SQLCHAR dbEmail[100]; // Adjust size as needed

    std::wstring query = L"SELECT Email FROM UserInfo WHERE Username = '" + std::wstring(username.begin(), username.end()) + L"'";
    retcode = SQLExecDirect(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS or retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_CHAR, dbEmail, sizeof(dbEmail), NULL);
    }
    else {
        throw std::runtime_error("User not found or no Email available for '" + username + "'");
    }

    return std::string(reinterpret_cast<char*>(dbEmail));
}


void finalizeDatabase() {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
