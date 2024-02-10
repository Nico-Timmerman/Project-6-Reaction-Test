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

// Helper functions that will serve the various static content of the website
void sendHTML(response& res, string filename) { sendFile(res, filename + ".html", "text/html"); }
void sendImage(response& res, string filename) { sendFile(res, "images/" + filename, "image/jpeg"); }
void sendScript(response& res, string filename) { sendFile(res, "scripts/" + filename, "text/javascript"); }
void sendStyle(response& res, string filename) { sendFile(res, "styles/" + filename, "text/css"); }


int main() {
    SimpleApp app;

	//ROOT ROUTE - Default login.html page
	CROW_ROUTE(app, "/")
		([](const request& req, response& res) { sendHTML(res, "login"); });

	// defines a generic route for an HTML pages
	CROW_ROUTE(app, "/<string>")
		([](const request& req, response& res, string filename)
			{
				sendHTML(res, filename);
			});

	/**** The following routes support the database-UserCreation capabilities for the web server***/
	CROW_ROUTE(app, "/Event/<string>").methods(HTTPMethod::POST)
		([](const request& req, response& res, string EventType) 
			{
				string username = "";
				string password = "";
				string email = "";
				
				string Method = method_name(req.method);
				if (Method == "POST") {
					// The POST path here should be used to check the database for the user and handle the login.
					string username = req.url_params.get("username");
					string password = req.url_params.get("password");
				}
				
				string msg = "";		//Generic response message to be generated based on request
				bool result = false;	//Result of writing event to the log file

				if (EventType == "Login"){
					// Once the Database has users in it that we can check against the following line should be switched
					// Such that it checks if the user exists in the Database. For now it checks if user=admin & pass=admin.
					if (username == "admin" && password == "admin"){
						sendFile(res, "ReactionTest.html?username=admin&password=admin", "text/html");
						result = true;
					}
				} else {
					res.code = 400;
					msg = "Bad Request.  No Event Type " + EventType;
					res.write(msg.c_str());
				}

				if (result) {
					res.code = 202;
					msg = "<div>Event Added Successfully <a href=\"http://localhost:23500\"> RETURN HOME </a></div>";
					res.write(msg.c_str());
				} else {
					res.code = 500;
					msg = "INTERNAL SERVER ERROR";
					res.write(msg.c_str());
				}

				res.end();
			});

	/* The methods in this route are methods that cannot be called within HTML due to general lacking-
	 * browser support. OPTIONS is handled by Crow by default, as noted below this route in comments.
	 * 
	 * NEED TO FIND A WAY TO CALL THESE ROUTES (can be called easily in JMeter as far as I know).
	*/
	CROW_ROUTE(app, "/Event/<string>").methods(HTTPMethod::DELETE, HTTPMethod::PATCH, HTTPMethod::PUT)
		([](const request& req, response& res, string EventType)
			{
				string username = "";
				string password = "";
				string email = "";
				string highscore = "";

				string Method = method_name(req.method);
				if (Method == "DELETE") {
					// The DELETE method here should be used to remove a user from the Database
					username = req.url_params.get("username");
				} else if (Method == "PUT") {
					// The PUT path here should be used to add a new user to the database.
					username = req.url_params.get("username");
					password = req.url_params.get("password");
					email = req.url_params.get("email");
				} else {
					// The PATCH/Update path here should be used to update the user's score in the Database
					username = req.url_params.get("username");
					highscore = req.url_params.get("highscore");
				}

				string msg = "";		//Generic response message to be generated based on request
				bool result = false;	//Result of writing event to the log file

				if (EventType == "PruneUser") {
					deleteUser(username);
					result = true;
				} else if (EventType == "UpdateScore") {
					updateHighScore(username, stoi(highscore));
					result = true;
				} else if (EventType == "CreateUser") {
					addUser(username, email, password);
					result = true;
				} else {
					res.code = 400;
					msg = "Bad Request.  No Event Type, receieved: " + EventType;
					res.write(msg.c_str());
				}

				if (result) {
					res.code = 202;
					msg = "<div>Event Successfully Updated <a href=\"http://localhost:23500\"> RETURN HOME </a></div>";
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
					msg = "<div>Event Successfully Updated <a href=\"http://localhost:23500\"> RETURN HOME </a></div>";
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
	*?

    /********* Standard routes to support styles and images *********/
	CROW_ROUTE(app, "/styles/<string>")([](const request& req, response& res, string filename) { sendStyle(res, filename); });
	CROW_ROUTE(app, "/images/<string>")([](const request& req, response& res, string filename) { sendImage(res, filename); });


    // Start the server on port 23500
    app.port(23500).multithreaded().run();

	finalizeDatabase();
    return 0;
}

