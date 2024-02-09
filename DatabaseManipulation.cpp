#include "DatabaseManipulation.h"
#include <sql.h>
#include <sqlext.h>
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
int initializeDatabase() {
    SQLRETURN retcode;

    // Allocate environment handle
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        return -1; // Failed to allocate environment handle
    }

    // Set the ODBC version environment attribute
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        return -2; // Failed to set ODBC version environment attribute
    }

    // Allocate connection handle
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        return -3; // Failed to allocate connection handle
    }

    // Connection string
    std::wstring connStr = L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=project6group5-server.database.windows.net,1433;DATABASE=Project6Group5DB;UID=group5Guest;PWD=gr0up5Gu3st;";

    // Connect to the database
    SQLSMALLINT outConnStrLength;
    retcode = SQLDriverConnectW(hdbc, NULL, narrowToWide(connStr.c_str()), SQL_NTS, NULL, 0, &outConnStrLength, SQL_DRIVER_NOPROMPT);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        return -4; // Failed to connect to the database
    }

    // Allocate statement handle
    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLDisconnect(hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
        return -5; // Failed to allocate statement handle
    }

    return 0; // Success
}

// Get high score for a user
int getHighScore(const std::string& username, int& highScore) {
    SQLRETURN retcode;

    std::wstring query = L"SELECT HighScore FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, narrowToWide(query.c_str()), SQL_NTS);

    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_SLONG, &highScore, 0, NULL);
        return 0; // Success
    }

    return -6; // User not found or no high score available
}

// Get password for a user
int getPassword(const std::string& username, std::string& password) {
    SQLRETURN retcode;
    SQLCHAR dbPassword[100];
    SQLLEN ind;

    std::wstring query = L"SELECT Password FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, narrowToWide(query.c_str()), SQL_NTS);

    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_CHAR, dbPassword, sizeof(dbPassword), &ind);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            if (ind != SQL_NULL_DATA) {
                password.assign(reinterpret_cast<char*>(dbPassword));
                return 0; // Success
            }
            return -7; // Password is NULL
        }
        return -8; // Failed to get password
    }

    return -6; // User not found or no password available
}

// Get email for a user
int getEmail(const std::string& username, std::string& email) {
    SQLRETURN retcode;
    SQLCHAR dbEmail[100];
    SQLLEN ind;

    std::wstring query = L"SELECT Email FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, narrowToWide(query.c_str()), SQL_NTS);

    retcode = SQLFetch(hstmt);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLGetData(hstmt, 1, SQL_C_CHAR, dbEmail, sizeof(dbEmail), &ind);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            if (ind != SQL_NULL_DATA) {
                email.assign(reinterpret_cast<char*>(dbEmail));
                return 0; // Success
            }
            return -9; // Email is NULL
        }
        return -10; // Failed to get email
    }

    return -6; // User not found or no email available
}

// Add a new user
int addUser(const std::string& username, const std::string& email, const std::string& password) {
    SQLRETURN retcode;

    std::wstring query = L"INSERT INTO UserInfo (Username, Email, Password, HighScore) VALUES ('" + narrowToWide(username) + L"', '" + narrowToWide(email) + L"', '" + narrowToWide(password) + L"', 0)";
    retcode = SQLExecDirectW(hstmt, narrowToWide(query.c_str()), SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        return 0; // Success
    }

    return -11; // Failed to add new user
}

// Update high score for a user
int updateHighScore(const std::string& username, int newHighScore) {
    SQLRETURN retcode;

    std::wstring query = L"UPDATE UserInfo SET HighScore = " + std::to_wstring(newHighScore) + L" WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, narrowToWide(query.c_str()), SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        return 0; // Success
    }

    return -12; // Failed to update high score
}

// Delete a user
int deleteUser(const std::string& username) {
    SQLRETURN retcode;

    std::wstring query = L"DELETE FROM UserInfo WHERE Username = '" + narrowToWide(username) + L"'";
    retcode = SQLExecDirectW(hstmt, narrowToWide(query.c_str()), SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        return 0; // Success
    }

    return -13; // Failed to delete user
}

// Finalize database
void finalizeDatabase() {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}