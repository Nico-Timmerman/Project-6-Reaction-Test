#ifndef DATABASE_MANIPULATION_H
#define DATABASE_MANIPULATION_H

#include <iostream>
#include <cstdint>

//#include <Windows.h> - Ive taken some definitions from this header
/*
typedef wchar_t* LPWSTR;
typedef uint32_t DWORD;
typedef void VOID;
typedef bool BOOL;
typedef int64_t INT64;
typedef uint64_t UINT64;
typedef uint16_t WORD;
typedef uint8_t BYTE;

struct GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
};

typedef wchar_t WCHAR; //This line and some others were giving errors
typedef char CHAR;
*/


#include <sql.h>
#include <sqlext.h>
#include <stdexcept>
#include <string>
#include <codecvt>
#include <locale>

void initializeDatabase();
int getHighScore(const std::string& username);
std::string getPassword(const std::string& username);
std::string getEmail(const std::string& username);
void updateHighScore(const std::string& username, int newHighScore);
void addUser(const std::string& username, const std::string& email, const std::string& password);
void deleteUser(const std::string& username);
void finalizeDatabase();

#endif // DATABASE_MANIPULATION_H