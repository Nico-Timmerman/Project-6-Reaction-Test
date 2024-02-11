#include "DatabaseManipulation.h"
#include <sql.h>
#include <sqlext.h>
#include <stdexcept>
#include <string>

// Global variables for database handles
SQLHENV henv = SQL_NULL_HENV;
SQLHDBC hdbc = SQL_NULL_HDBC;
SQLHSTMT hstmt = SQL_NULL_HSTMT;

void initializeDatabase() {
    SQLRETURN retcode;

    // Allocate an environment handle
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        throw std::runtime_error("Failed to allocate environment handle");
    }

    // Set the ODBC version to 3.0
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw std::runtime_error("Failed to set ODBC version environment attribute");
    }

    // Allocate a connection handle
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw std::runtime_error("Failed to allocate connection handle");
    }

    // Define the connection string
    std::string connStr = "Driver={ODBC Driver 17 for SQL Server};"
        "Server=tcp:project6group5-server.database.windows.net,1433;"
        "Database=Project6Group5DB;"
        "Uid=group5Guest;"
        "Pwd=gr0up5Gu3st;"
        "Encrypt=yes;"
        "TrustServerCertificate=no;"
        "Connection Timeout=30;";

    SQLSMALLINT outConnStrLength = 0;

    // Connect to the database
    retcode = SQLDriverConnect(hdbc, NULL, (SQLCHAR*)connStr.c_str(), SQL_NTS, NULL, 0, &outConnStrLength, SQL_DRIVER_NOPROMPT);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw std::runtime_error("Failed to connect to the database");
    }
}

int getHighScore(const std::string& username) {
    SQLRETURN retcode;
    std::string query = "SELECT HighScore FROM UserInfo WHERE Username = '" + username + "'";
    retcode = SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    int highScore = -1;
    if (SQL_SUCCEEDED(retcode)) {
        if (SQL_SUCCEEDED(SQLFetch(hstmt))) { // Ensure SQLFetch is called before SQLGetData
            SQLGetData(hstmt, 1, SQL_C_SLONG, &highScore, 0, NULL);
        }
    }

    return highScore;
}

std::string getPassword(const std::string& username) {
    SQLCHAR dbPassword[100];
    SQLLEN ind;
    std::string query = "SELECT Password FROM UserInfo WHERE Username = '" + username + "'";
    SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    std::string password;
    if (SQL_SUCCEEDED(SQLFetch(hstmt))) {
        SQLGetData(hstmt, 1, SQL_C_CHAR, dbPassword, sizeof(dbPassword), &ind);
        if (ind != SQL_NULL_DATA) password.assign(reinterpret_cast<char*>(dbPassword));
    }

    return password;
}

std::string getEmail(const std::string& username) {
    SQLCHAR dbEmail[100];
    SQLLEN ind;
    std::string query = "SELECT Email FROM UserInfo WHERE Username = '" + username + "'";
    SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    std::string email;
    if (SQL_SUCCEEDED(SQLFetch(hstmt))) {
        SQLGetData(hstmt, 1, SQL_C_CHAR, dbEmail, sizeof(dbEmail), &ind);
        if (ind != SQL_NULL_DATA) email.assign(reinterpret_cast<char*>(dbEmail));
    }

    return email;
}

int addUser(const std::string& username, const std::string& email, const std::string& password) {
    std::string query = "INSERT INTO UserInfo (Username, Email, Password, HighScore) VALUES ('" +
                        username + "', '" + email + "', '" + password + "', 0)";
    SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    return SQL_SUCCEEDED(SQLFetch(hstmt)) ? 0 : -1;
}

int updateHighScore(const std::string& username, int newHighScore) {
    std::string query = "UPDATE UserInfo SET HighScore = " + std::to_string(newHighScore) +
                        " WHERE Username = '" + username + "'";
    SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    return SQL_SUCCEEDED(SQLFetch(hstmt)) ? 0 : -1;
}

int deleteUser(const std::string& username) {
    std::string query = "DELETE FROM UserInfo WHERE Username = '" + username + "'";
    SQLExecDirect(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    return SQL_SUCCEEDED(SQLFetch(hstmt)) ? 0 : -1;
}

// Finalize database
void finalizeDatabase() {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
