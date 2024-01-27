#ifndef DATABASE_MANIPULATION_H
#define DATABASE_MANIPULATION_H

#include <iostream>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdexcept>
#include <string>


void initializeDatabase();
int getHighScore(const std::string& username);
std::string getPassword(const std::string& username);
std::string getEmail(const std::string& username);
void finalizeDatabase();

#endif // DATABASE_MANIPULATION_H
