#include "DatabaseManipulation.h"
#include <sql.h>
#include <sqlext.h>
#include <stdexcept>
#include <string>
#include <codecvt>

// Global variables for database handles
SQLHENV henv;
SQLHDBC hdbc;
SQLHSTMT hstmt;

// Function to convert from narrow to wide string
std::wstring narrowToWide(const std::string& narrowStr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(narrowStr);
}

// Initialize database
void initializeDatabase() {
    SQLRETURN retcode;

    // Allocate environment handle
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        throw std::runtime_error("Failed to allocate environment handle");
    }

    // Set the ODBC version environment attribute
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw std::runtime_error("Failed to set ODBC version environment attribute");
    }

    // Allocate connection handle
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw std::runtime_error("Failed to allocate connection handle");
    }

    // Connection string already a wide string, so no need to convert
    std::wstring connStr = L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=project6group5-server.database.windows.net,1433;DATABASE=Project6Group5DB;UID=group5Guest;PWD=gr0up5Gu3st;";

    // Connect to the database
    SQLSMALLINT outConnStrLength;
    retcode = SQLDriverConnectW(hdbc, NULL, (SQLWCHAR*)connStr.c_str(), SQL_NTS, NULL, 0, &outConnStrLength, SQL_DRIVER_NOPROMPT);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw std::runtime_error("Failed to connect to the database");
    }

    // Allocate statement handle
    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLDisconnect(hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        throw std::runtime_error("Failed to allocate statement handle");
    }
}

// Get high score for a user
int getHighScore(const std::string& username) {
    SQLRETURN retcode;

    std::wstring query = L"SELECT HighScore FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    int highScore = -1; // Default value if user not found or no high score available
    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_SLONG, &highScore, 0, NULL);
    }

    return highScore;
}

// Get password for a user
std::string getPassword(const std::string& username) {
    SQLRETURN retcode;
    SQLCHAR dbPassword[100];
    SQLLEN ind;

    std::wstring query = L"SELECT Password FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    std::string password; // Default value if user not found or password is NULL
    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_CHAR, dbPassword, sizeof(dbPassword), &ind);
        if (ind != SQL_NULL_DATA) {
            password.assign(reinterpret_cast<char*>(dbPassword));
        }
    }

    return password;
}

// Get email for a user
std::string getEmail(const std::string& username) {
    SQLRETURN retcode;
    SQLCHAR dbEmail[100];
    SQLLEN ind;

    std::wstring query = L"SELECT Email FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    std::string email; // Default value if user not found or email is NULL
    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_CHAR, dbEmail, sizeof(dbEmail), &ind);
        if (ind != SQL_NULL_DATA) {
            email.assign(reinterpret_cast<char*>(dbEmail));
        }
    }

    return email;
}

// Add a new user
int addUser(const std::string& username, const std::string& email, const std::string& password) {
    SQLRETURN retcode;

    std::wstring query = L"INSERT INTO UserInfo (Username, Email, Password, HighScore) VALUES ('" + narrowToWide(username) + L"', '" + narrowToWide(email) + L"', '" + narrowToWide(password) + L"', 0)";
    retcode = SQLExecDirectW(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        return 0; // Success
    }

    return -1; // Failed to add new user
}

// Update high score for a user
int updateHighScore(const std::string& username, int newHighScore) {
    SQLRETURN retcode;

    std::wstring query = L"UPDATE UserInfo SET HighScore = " + std::to_wstring(newHighScore) + L" WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        return 0; // Success
    }

    return -1; // Failed to update high score
}

// Delete a user
int deleteUser(const std::string& username) {
    SQLRETURN retcode;

    std::wstring query = L"DELETE FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        return 0; // Success
    }

    return -1; // Failed to delete user
}

// Finalize database
void finalizeDatabase() {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
