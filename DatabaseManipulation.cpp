#include "DatabaseManipulation.h"
#include <iostream>
#include <stdexcept>

// Constructor
DatabaseManipulation::DatabaseManipulation() {
    std::cout << "Initializing database..." << std::endl;
    if (sqlite3_open("mydatabase.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        std::cout << "Database opened successfully." << std::endl;
    }
    initializeDatabase(); // Assuming this sets up tables, etc.
}

// Destructor
DatabaseManipulation::~DatabaseManipulation() {
    sqlite3_close(db);
}

// Initialize the database and create tables if they don't exist
void DatabaseManipulation::initializeDatabase() {
    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY, "
        "email TEXT NOT NULL, "
        "password TEXT NOT NULL);"
        "CREATE TABLE IF NOT EXISTS scores ("
        "username TEXT PRIMARY KEY, "
        "high_score INTEGER NOT NULL, "
        "FOREIGN KEY(username) REFERENCES users(username));";

    char* errorMessage;
    if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        throw std::runtime_error("Failed to create tables: " + std::string(errorMessage));
        sqlite3_free(errorMessage);
    }

    seedDatabase(); // Seed the database with initial data
}

// Seed the database with test data
void DatabaseManipulation::seedDatabase() {
    const char* seedDataSQL =
        "INSERT OR IGNORE INTO users (username, email, password) VALUES "
        "('testUser', 'testEmail@example.com', 'testPassword'),"
        "('sampleUser', 'sampleEmail@example.com', 'samplePassword');"
        "INSERT OR IGNORE INTO scores (username, high_score) VALUES "
        "('testUser', 1000),"
        "('sampleUser', 2000);";

    char* errorMessage;
    if (sqlite3_exec(db, seedDataSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        throw std::runtime_error("Failed to seed database: " + std::string(errorMessage));
        sqlite3_free(errorMessage);
    }
}

// Add a new user to the database
bool DatabaseManipulation::addUser(const std::string& username, const std::string& email, const std::string& password) {
    const char* insertSQL = "INSERT INTO users (username, email, password) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
// Get the high score for a given user
int DatabaseManipulation::getHighScore(const std::string& username) {
    const char* query = "SELECT high_score FROM scores WHERE username = ?;";
    sqlite3_stmt* stmt;
    int highScore = -1; // Default value if not found

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            highScore = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    return highScore;
}

// Get the password for a given user
std::string DatabaseManipulation::getPassword(const std::string& username) {
    const char* query = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    std::string password; // Empty string if not found

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    return password;
}

// Update the high score for a given user
bool DatabaseManipulation::updateHighScore(const std::string& username, int newHighScore) {
    const char* query = "UPDATE scores SET high_score = ? WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, newHighScore);
        sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

// Delete a user from the database
bool DatabaseManipulation::deleteUser(const std::string& username) {
    const char* query = "DELETE FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

void DatabaseManipulation::finalizeDatabase() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr; // Ensure the pointer is no longer used
    }
}


