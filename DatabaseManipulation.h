#ifndef DATABASE_MANIPULATION_H
#define DATABASE_MANIPULATION_H

#include <iostream>
#include <Windows.h>
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
