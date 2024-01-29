#define CROW_MAIN

#include "crow_all.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;
using namespace crow;
using namespace web;
using namespace web::http;
using namespace web::http::client;

const utility::string_t connection_string = U("<connection_string>");

struct UserData {
    int UserID;
    utility::string_t Username;
    utility::string_t Password;
    utility::string_t Email;
    int HighScore;
};

// In-memory storage for user data (this should be replaced with a database)
std::vector<UserData> users;

// Function to insert user data into the Azure SQL Database
void insertUserData(const UserData& user) {
    try {
        // Create a connection to the Azure SQL Database
        sql::SqlConnection conn(connection_string);

        // Open the connection
        conn.Open();

        // Define the SQL query to insert user data
        const wstring query = L"INSERT INTO Users (UserID, Username, Password, Email, HighScore) "
            L"VALUES (?, ?, ?, ?, ?)";

        // Create a command object
        sql::SqlCommand cmd(conn, query);

        // Set parameter values
        cmd.Parameters().AddInt32(L"UserID", user.UserID);
        cmd.Parameters().AddString(L"Username", user.Username);
        cmd.Parameters().AddString(L"Password", user.Password);
        cmd.Parameters().AddString(L"Email", user.Email);
        cmd.Parameters().AddInt32(L"HighScore", user.HighScore);

        // Execute the SQL command
        cmd.ExecuteNonQuery();
    }
    catch (const sql::SqlException& e) {
        cerr << "SQL Exception: " << e.what() << endl;
    }
}

// Function to find a user by username in the in-memory storage
UserData* findUserByUsername(const utility::string_t& username) {
    auto it = find_if(users.begin(), users.end(), [username](const UserData& user) {
        return user.Username == username;
        });

    return (it != users.end()) ? &(*it) : nullptr;
}

// Function to handle user registration (now allows for PUT requests)
void handleRegistration(const request& req, response& res) {
    utility::string_t username = req.body["username"];
    utility::string_t password = req.body["password"];
    utility::string_t email = req.body["email"];

    // Check if the username is already taken in the in-memory storage
    UserData* existingUser = findUserByUsername(username);

    if (existingUser != nullptr && req.method == HTTPMethod::Put) {
        // Update existing user data
        existingUser->Password = password;
        existingUser->Email = email;

        // Insert the updated user data into the Azure SQL Database
        insertUserData(*existingUser);

        // Send a JSON response with the updated user data
        res.set_header("Content-Type", "application/json");
        res.write("{\"message\": \"User information updated successfully\", \"user\": " + to_string(existingUser->UserID) + "}");
        res.end();
    }
    else if (existingUser == nullptr) {
        // Create a new user
        UserData newUser;
        newUser.UserID = users.size() + 1;
        newUser.Username = username;
        newUser.Password = password; // In a real application, hash and salt the password
        newUser.Email = email;
        newUser.HighScore = 0;

        // Add the new user to the in-memory storage
        users.push_back(newUser);

        // Insert the new user data into the Azure SQL Database
        insertUserData(newUser);

        // Send a JSON response with the user data
        res.set_header("Content-Type", "application/json");
        res.write("{\"message\": \"User registered successfully\", \"user\": " + to_string(newUser.UserID) + "}");
        res.end();
    }
    else {
        res.code = 400; // Bad Request
        res.write("Username already exists");
        res.end();
    }
}

// Function to handle user login (now allows for PUT requests)
void handleLogin(const request& req, response& res) {
    utility::string_t username = req.body["username"];
    utility::string_t password = req.body["password"];

    // Find the user by username in the in-memory storage
    UserData* user = findUserByUsername(username);

    // Check if the user exists and the password is correct
    if (user != nullptr && user->Password == password) {
        // Send a JSON response with the user data
        res.set_header("Content-Type", "application/json");
        res.write("{\"message\": \"Login successful\", \"user\": " + to_string(user->UserID) + "}");
        res.end();
    }
    else if (user != nullptr && req.method == HTTPMethod::Put) {
        // Update the user's password
        user->Password = password;

        // Insert the updated user data into the Azure SQL Database
        insertUserData(*user);

        // Send a JSON response with the updated user data
        res.set_header("Content-Type", "application/json");
        res.write("{\"message\": \"Password updated successfully\", \"user\": " + to_string(user->UserID) + "}");
        res.end();
    }
    else {
        res.code = 401; // Unauthorized
        res.write("Invalid username or password");
        res.end();
    }
}

// Function to handle high score updates (now allows for PUT requests)
void handleHighScore(const request& req, response& res) {
    utility::string_t username = req.body["username"];
    int newHighScore = std::stoi(req.body["highscore"]);

    // Find the user by username in the in-memory storage
    UserData* user = findUserByUsername(username);

    // Check if the user exists
    if (user != nullptr) {
        // Update the user's high score
        user->HighScore = newHighScore;

        // Insert the updated user data into the Azure SQL Database
        insertUserData(*user);

        // Send a JSON response with the updated user data
        res.set_header("Content-Type", "application/json");
        res.write("{\"message\": \"High score updated successfully\", \"user\": " + to_string(user->UserID) + "}");
        res.end();
    }
    // ... (existing code above)

    else {
        res.code = 404; // Not Found
        res.write("User not found");
        res.end();
    }
}

int main() {
    SimpleApp app;

    // Register routes
    CROW_ROUTE(app, "/register").methods(HTTPMethod::Post, HTTPMethod::Put)(handleRegistration);
    CROW_ROUTE(app, "/login").methods(HTTPMethod::Post, HTTPMethod::Put)(handleLogin);
    CROW_ROUTE(app, "/highscore").methods(HTTPMethod::Post, HTTPMethod::Put)(handleHighScore);

    // ... (other routes)

    // Serve HTML pages
    CROW_ROUTE(app, "/<string>").methods(HTTPMethod::Get)([](const request& req, response& res, string filename) {
        ifstream in("../public/" + filename, ifstream::in);
        if (in) {
            ostringstream contents;
            contents << in.rdbuf();
            in.close();
            res.set_header("Content-Type", "text/html");
            res.write(contents.str());
            res.code = 200;
        }
        else {
            res.code = 404;
            res.write("Not Found");
        }
        res.end();
        });

    // Start the server on port 23500
    app.port(23500).multithreaded().run();

    return 0;
}

