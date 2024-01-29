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
    SQLCHAR dbPassword[100];

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
    SQLCHAR dbEmail[100];

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

void addUser(const std::string& username, const std::string& email, const std::string& password) {
    SQLRETURN retcode;

    // Convert std::string to std::wstring
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wUsername = converter.from_bytes(username);
    std::wstring wEmail = converter.from_bytes(email);
    std::wstring wPassword = converter.from_bytes(password);

    std::wstring query = L"INSERT INTO UserInfo (Username, Email, Password, HighScore) VALUES (?, ?, ?, 0)";
    retcode = SQLPrepare(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    // Bind parameters
    SQLLEN lenUsername = wUsername.size() * sizeof(wchar_t);
    SQLLEN lenEmail = wEmail.size() * sizeof(wchar_t);
    SQLLEN lenPassword = wPassword.size() * sizeof(wchar_t);
    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 0, 0, (SQLWCHAR*)wUsername.c_str(), lenUsername, &lenUsername);
    retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 0, 0, (SQLWCHAR*)wEmail.c_str(), lenEmail, &lenEmail);
    retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 0, 0, (SQLWCHAR*)wPassword.c_str(), lenPassword, &lenPassword);

    retcode = SQLExecute(hstmt);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        throw std::runtime_error("Failed to add new user");
    }
}


void updateHighScore(const std::string& username, int newHighScore) {
    SQLRETURN retcode;

    // Convert std::string to std::wstring
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wUsername = converter.from_bytes(username);

    std::wstring query = L"UPDATE UserInfo SET HighScore = ? WHERE Username = ?";
    retcode = SQLPrepare(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    // Bind parameters
    SQLLEN lenHighScore = sizeof(newHighScore);
    SQLLEN lenUsername = wUsername.size() * sizeof(wchar_t);
    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &newHighScore, lenHighScore, &lenHighScore);
    retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 0, 0, (SQLWCHAR*)wUsername.c_str(), lenUsername, &lenUsername);

    retcode = SQLExecute(hstmt);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        throw std::runtime_error("Failed to update high score for user '" + username + "'");
    }
}

void deleteUser(const std::string& username) {
    SQLRETURN retcode;

    // Convert std::string to std::wstring for SQL compatibility
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wUsername = converter.from_bytes(username);

    std::wstring query = L"DELETE FROM UserInfo WHERE Username = ?";
    retcode = SQLPrepare(hstmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

    // Bind the username parameter to prevent SQL injection
    SQLLEN lenUsername = wUsername.size() * sizeof(wchar_t);
    retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 0, 0, (SQLWCHAR*)wUsername.c_str(), lenUsername, &lenUsername);

    // Execute the query
    retcode = SQLExecute(hstmt);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        throw std::runtime_error("Failed to delete user '" + username + "'");
    }
}

void finalizeDatabase() {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    SQLDisconnect(hdbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
