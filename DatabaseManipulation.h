#ifndef DATABASE_MANIPULATION_H
#define DATABASE_MANIPULATION_H

#include <iostream>
#include <cstdint>

//#include <Windows.h> - Ive taken some definitions from this header in the form of
// Conditional typedefs to avoid redefinition errors
#ifndef DWORD
typedef uint32_t DWORD;
#endif

#ifndef VOID
typedef void VOID;
#endif

#ifndef BOOL
typedef bool BOOL;
#endif

#ifndef INT64
typedef int64_t INT64;
#endif

#ifndef UINT64
typedef uint64_t UINT64;
#endif

#ifndef WORD
typedef uint16_t WORD;
#endif

#ifndef BYTE
typedef uint8_t BYTE;
#endif

// GUID structure
#ifndef GUID_DEFINED
#define GUID_DEFINED
struct GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
};
#endif

#ifndef WCHAR
typedef short unsigned int WCHAR;
#endif

#ifndef LPWSTR
typedef WCHAR* LPWSTR;
#endif

#ifndef CHAR
typedef char CHAR;
#endif


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
int addUser(const std::string& username, const std::string& email, const std::string& password);
int updateHighScore(const std::string& username, int newHighScore);
int deleteUser(const std::string& username);
void finalizeDatabase();

#endif // DATABASE_MANIPULATION_H