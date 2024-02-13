#define CROW_MAIN

#include "crow_all.h"
#include "DatabaseManipulation.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace crow;

struct UserData {
    int UserID;
    string Username;
    string Password;
    string Email;
    int HighScore;
} user;

string const_username = "username";
string const_password = "password";
string const_email = "email";

// Handler for styles.css file and simple hard routes
void sendFile(response& res, string filename, string contentType)
{
    ifstream in("../public/" + filename, ifstream::in);
    if (in)
    {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", contentType);
        res.write(contents.str());
        res.code = 200;
    }
    else
    {
        res.code = 404;
        res.write("Not Found");
    }
    res.end();
}

// Helper functions that will serve the various static content of the website
void sendHTML(response& res, string filename) { sendFile(res, filename + ".html", "text/html"); }
void sendImage(response& res, string filename) { sendFile(res, "images/" + filename, "image/jpeg"); }
void sendScript(response& res, string filename) { sendFile(res, "scripts/" + filename, "text/javascript"); }
void sendStyle(response& res, string filename) { sendFile(res, "styles/" + filename, "text/css"); }

int main()
{
    SimpleApp app;

    // Create an instance of DatabaseManipulation
    DatabaseManipulation db;
    db.initializeDatabase();


    // Set log level to
    app.loglevel(LogLevel::Info);

    // Set log handler to console
    // ROOT ROUTE - Default login.html page
    CROW_ROUTE(app, "/")([&db](const request& req, response& res) { sendHTML(res, "login"); });

    // defines a generic route for an HTML pages
    CROW_ROUTE(app, "/<string>")([&db](const request& req, response& res, string filename)
        { sendHTML(res, filename); });

    /**** The following routes support the database-UserCreation capabilities for the web server***/
    CROW_ROUTE(app, "/Event/<string>")
        .methods(HTTPMethod::POST)([&db](const request& req, response& res, string EventType) {
        string username = "";
        string password = "";

        string Method = method_name(req.method);
        if (Method == "POST")
        {
            crow::json::rvalue json_body;
            json_body = crow::json::load(req.body);

            username = json_body["username"].s();
            password = json_body["password"].s();
        }

        string msg = ""; //Generic response message to be generated based on request
        bool result = false; //Result of writing event to the log file

        if (EventType == "Login")
        {
            string passCheck = db.getPassword(username);

            if (password == passCheck)
            {
                sendHTML(res, "ReactionTest");
                result = true;

                res.code = 200;
                res.write(""); // Empty response body
            }
            else
            {
                res.code = 401;
                msg = "Invalid Credentials";
                res.write(msg.c_str());
            }
        }
        else
        {
            res.code = 400;
            msg = "Bad Request.  No Event Type " + EventType;
            res.write(msg.c_str());
        }

        if (result) {
            res.code = 202;
            msg = "<div> Login Successful! <a href=\"http://localhost:23500\"> RETURN HOME </a></div>";
            res.write(msg.c_str());
        } else {
            res.code = 500;
            msg = "INTERNAL SERVER ERROR";
            res.write(msg.c_str());
        }

        res.end();
        });


	CROW_ROUTE(app, "/Request/Highscore").methods(HTTPMethod::GET)
			([&db](const request& req, response& res)
			{

				string username = "";
				int highscore; 

				username = req.url_params.get("username");
				highscore = db.getHighScore(username);
				
                //CROW_LOG_ERROR << highscore;

				res.set_header("Content-Type", "text/plain");
                res.write(std::to_string(highscore));
                res.end();
			});
		
	
		

	/* The methods in this route are methods that cannot be called within HTML due to general lacking-
	 * browser support. OPTIONS is handled by Crow by default, as noted below this route in comments.
	 * 
	 * These routes are to be called in JMeter or other programs 
	 * HTML cannot call methods other than GET or POST
	*/
	CROW_ROUTE(app, "/Event/<string>").methods(HTTPMethod::DELETE, HTTPMethod::PATCH, HTTPMethod::PUT)
		([&db](const request& req, response& res, string EventType)
			{
				string username = "";
				string password = "";
				string email = "";
				string highscore = "";

            string Method = method_name(req.method);
            CROW_LOG_INFO << Method;
            if (Method == "DELETE")
            {
                username = req.url_params.get("username");
            }
            else if (Method == "PUT")
            {
                username = req.url_params.get("username");
                password = req.url_params.get("password");
                email = req.url_params.get("email");
            }
            else
            {
                CROW_LOG_INFO << "Inside Else Clause";
                crow::json::rvalue json_body;
                json_body = crow::json::load(req.body);

                username = json_body["username"].s();
                highscore = json_body["score"].s();

                CROW_LOG_INFO << username + highscore;
            }

            string msg = ""; //Generic response message to be generated based on request
            bool result = false; //Result of writing event to the log file

				if (EventType == "PruneUser") {
					db.deleteUser(username);
					result = true;
				} else if (EventType == "UpdateScore") {
					db.updateHighScore(username, stoi(highscore));
					result = true;

                    res.code = 200;
                    res.write("");
				} else if (EventType == "CreateUser") {
					db.addUser(username, email, password);
					result = true;
				} else {
					res.code = 400;
					msg = "Bad Request.  No Event Type, receieved: " + EventType;
					res.write(msg.c_str());
				}

				if (result) {
					res.code = 202;
					msg = "<div>Action Successful! <a href=\"http://localhost:23500\"> RETURN HOME </a></div>";
					res.write(msg.c_str());
				} else {
					res.code = 500;
					msg = "INTERNAL SERVER ERROR";
					res.write(msg.c_str());
				}

				res.end();
			});

	/*
	// Ignore this code!
	// Crow handles Options automatically!
	// https://crowcpp.org/master/guides/routes/
	CROW_ROUTE(app, "/Event/<string>").methods(HTTPMethod::OPTIONS)
		([](const request& req, response& res, string EventType)
			{
				if (EventType == "Options")
					result = true;
				else
				{
					res.code = 400;
					msg = "Bad Request.  No Event Type, receieved: " + EventType;
					res.write(msg.c_str());
				}
				
				if (result)
				{
					res.code = 202;
					msg = "<div> Success! <a href=\"http://localhost:23500\"> RETURN HOME </a></div>";
					res.write(msg.c_str());
				}
				else
				{
					res.code = 500;
					msg = "INTERNAL SERVER ERROR";
					res.write(msg.c_str());
				}

            res.end();
        });

    /********* Standard routes to support styles and images *********/
    CROW_ROUTE(app, "/styles/<string>")([](const request& req, response& res, string filename) { sendStyle(res, filename); });
    CROW_ROUTE(app, "/images/<string>")([](const request& req, response& res, string filename) { sendImage(res, filename); });

    // Start the server on port 23500
    app.port(23500).multithreaded().run();

    db.finalizeDatabase();
    return 0;
}
