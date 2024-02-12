#ifndef DATABASEMANIPULATION_H
#define DATABASEMANIPULATION_H

#include <iostream>
#include <string>
#include <sqlite3.h> // Include SQLite

class DatabaseManipulation {
private:
    sqlite3* db; // SQLite database connection

public:
    DatabaseManipulation();
    ~DatabaseManipulation();
    void initializeDatabase();
    void finalizeDatabase();
    int getHighScore(const std::string& username);
    std::string getPassword(const std::string& username);
    bool addUser(const std::string& username, const std::string& email, const std::string& password);
    bool updateHighScore(const std::string& username, int newHighScore);
    bool deleteUser(const std::string& username);
    void seedDatabase(); // New method to seed the database
};

#endif // DATABASEMANIPULATION_H
