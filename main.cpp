#define CROW_MAIN

#include "crow_all.h"
#include "DatabaseManipulation.h"
#include <iostream>
#include <fstream>
#include <string>
//#include <cpprest/http_client.h>
//#include <cpprest/filestream.h>

using namespace std;
using namespace crow;


struct UserData {
    int UserID;
    string Username;
    string Password;
    string Email;
    int HighScore;
}user;

string const_username = "username";
string const_password = "password";
string const_email = "email";

/*
using namespace web;
using namespace web::http;
using namespace web::http::client;

const utility::string_t connection_string = U("<connection_string>");

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
    // ...

    else {
        res.code = 404; // Not Found
        res.write("User not found");
        res.end();
    }
}
*/

// Handler for styles.css file and simple hard routes
void sendFile (response& res, string filename, string contentType) {
	ifstream in("../public/" + filename, ifstream::in);
	if (in) {
		ostringstream contents;
		contents << in.rdbuf();
		in.close();
		res.set_header("Content-Type", contentType);
		res.write(contents.str());
		res.code = 200;
	}
	else {
		res.code = 404;
		res.write("Not Found");
	}
	res.end();
}

int main() {
    SimpleApp app;

    // Register routes
    //CROW_ROUTE(app, "/register").methods(HTTPMethod::Post, HTTPMethod::Put)(handleRegistration);
    //CROW_ROUTE(app, "/login").methods(HTTPMethod::Post, HTTPMethod::Put)(handleLogin);
    //CROW_ROUTE(app, "/highscore").methods(HTTPMethod::Post, HTTPMethod::Put)(handleHighScore);

	CROW_ROUTE(app, "/")
        ([](const request &req, response &res) {
            sendFile(res, "login.html", "text/html");
        });

	//Login Features
	  CROW_ROUTE(app, "/login").methods(HTTPMethod::Post)
        ([](const request &req, response &res) {
            string username = req.url_params.get("username");
            string password = req.url_params.get("password");

            // Check login credentials (you may want to replace this with your actual authentication logic)
            if (username == "your_username" && password == "your_password") {
                // Redirect to ReactionTest.html upon successful login
                res.redirect("/ReactionTest.html?username=" + username);
            } else {
                // Add logic for handling incorrect login credentials
                // For example, you might render an error message on the login.html page
                sendFile(res, "login.html", "text/html");
            }
        });

	//Route for ReactionTest.html
	CROW_ROUTE(app, "/ReactionTest.html").methods(HTTPMethod::Get)
        ([](const request &req, response &res) {
            sendFile(res, "ReactionTest.html", "text/html");
        });

	CROW_ROUTE(app, "/styles/<string>")
		([](const request& req, response& res, string filename) {
			sendFile(res, "styles/" + filename, "text/css");
		});

    //PATCH method Route
	//As I understand currently the idea is to update only part of the the user login resource(i.e. updating a users password but not username)
	//Updated to above, the PATCH method will be used to update the users highscore
	CROW_ROUTE(app, "/patchHighscore").methods(HTTPMethod::PATCH)
		([](const request& req, response& res) {
			string HS = req.url_params.get("testTime");
			int tmp_HS = stoi(HS);
			string uN = req.url_params.get("username");
			initializeDatabase();
			updateHighScore(uN, tmp_HS);
			if(req.method == HTTPMethod::PATCH){
				res.set_header("Content-Type", "application/json");
				res.write("Highscore updated!");
				res.code = 200;
			}
			else {
				res.code = 404;
				res.write("Not Found");
			}
			res.end();
		}); 
		
		
	//DELETE method Route
	//A good usage of the DELETE Method may be to "reset" a users high score, whether this be an admin-only tool, or available to all users is undetermined
	CROW_ROUTE(app, "/deleteUser").methods(HTTPMethod::DELETE)
		([](const request& req, response& res) {
			string uN = req.url_params.get("uN");
			initializeDatabase();
			deleteUser(uN);
			if(req.method == HTTPMethod::DELETE){
				res.set_header("Content-Type", "application/json");
				res.write("User Deleted!");
				res.code = 200;
			}
			else {
				res.code = 404;
				res.write("Not Found");
			}
			res.end();
		}); 
		
		
	//OPTIONS method Route
	//If my understanding is correct the technically this method should be allowed in addition to the others
	//E.g. this method shows what other methods are available, without acting on the resource, so maybe use it in conjunction
	CROW_ROUTE(app, "/options").methods(HTTPMethod::OPTIONS)
		([](const request& req, response& res) {
			//sendFile (response& res, string filename, string contentType) {
			if(req.method == HTTPMethod::OPTIONS){
				res.set_header("Content-Type", "application/json");
				res.write("Permitted: GET, POST, PUT, PATCH, DELETE, OPTIONS");
				res.code = 200;
			}
			else {
				res.code = 404;
				res.write("Not Found");
			}
			res.end();
		});
    // PUT route to update user information
    CROW_ROUTE(app, "/updateUser")
        .methods(HTTPMethod::PUT)
        ([&](const request& req, response& res) {
        try {
            string username = req.body/*[const_username]*/;
            string password = req.body/*[const_password]*/;
            string email = req.body/*[const_email]*/;

            // Update the user's information in the database
            addUser(username, password, email); //was originally updateUser but that does not exist yet

            // Send a JSON response with the updated user data
            res.set_header("Content-Type", "application/json");
            res.write("{\"message\": \"User information updated successfully\", \"user\": \"" + username + "\"}");
            res.end();
        }
        catch (const std::exception& e) {
            res.code = 400; // Bad Request
            res.write(e.what());
            res.end();
        }
            });

    // POST route to create a new user
    CROW_ROUTE(app, "/createUser")
        .methods(HTTPMethod::POST)
        ([&](const request& req, response& res) {
        try {
            string username = req.body/*[const_username]*/;
            string password = req.body/*[const_password]*/;
            string email = req.body/*[const_email]*/;

            // Create a new user in the database
            addUser(username, email, password);

            // Send a JSON response with the user data
            res.set_header("Content-Type", "application/json");
            res.write("{\"message\": \"User created successfully\", \"user\": \"" + username + "\"}");
            res.end();
        }
        catch (const std::exception& e) {
            res.code = 400; // Bad Request
            res.write(e.what());
            res.end();
        }
            });

    // GET route to retrieve user information
    CROW_ROUTE(app, "/getUser")
        .methods(HTTPMethod::GET)
        ([&](const request& req, response& res) {
        try {
            string username = req.url_params.get("username");

            // Retrieve user's information from the database
			//was originally getUser but we don't have that created yet
			//was also originally trying to populate the struct with the data but that functionality does not exist yet
            /*UserData user = */getHighScore(username); 
			

            // Send a JSON response with the user's information
            res.set_header("Content-Type", "application/json");
            res.write("{\"message\": \"User information retrieved successfully\", \"user\": \"" + username + "\", \"email\": \"" + user.Email + "\", \"highscore\": " + std::to_string(user.HighScore) + "}");
            res.end();
        }
        catch (const std::exception& e) {
            res.code = 404; // Not Found
            res.write(e.what());
            res.end();
        }
            });

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

	finalizeDatabase();
    return 0;
}

