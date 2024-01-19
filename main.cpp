#define CROW_MAIN

#include "crow_all.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace crow; // Saves us writing crow:: all the time


/*
 * LUCKILY Our purposes allow us to reuse much code from our previous personal website endeavour
 * While this is true, much of this code as of present needs to be stripped as it has little to do with our project.
 * 
 * In a update soon the back-end requirements that are not currently met will be added here, as of right now the back-end
 * -currently supports POST requests as well as GET requests
 * 
 * This code is reused from Mobile Networking Lab 10, when reading comments should any pertain to a "REQ #" see that asn document to understand it
 * It may not have been requested but adding data sanitation to user fields seems like it would also be a good idea (at least a little bit).
 * 
 * Things not relevant to this file:
 * The html pages, CSS, etc will all need to be changed and or updated to achieve our WebProject's goal.
 * E.g. Index.html should stay, but it's content should be updated to reflect our Project.
*/


//// Template Route
	//CROW_ROUTE(app, "")
	//	([](const request& req, response& res) {
	//		//Do stuff
	//	});


// This function chains off of our multiple helper functions that handle the TYPES (script/html/etc) 
// and functions as a pseudo-parent function to save code on repeated calls.
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

//This function parses all .html files EXCEPT the Index.html, because that file will have different routing due to it being the '/' default 
//It also takes html files from the root directory
void helperHTML (response& res, string filename) {
	sendFile(res, filename, "text/html"); // Invokes parent sendFile while while sending through our HTML file names separate to our other file/doc types
}

//This function receives and sends all styles folder .css files
void helperStyles(response& res, string filename) {
	sendFile(res, "styles/" + filename, "text/css"); // Specifies that we want to move out of the root/public directory into the styles directory
}

//This function receives and sends all scripts folder .js files
void helperScripts(response& res, string filename) {
	sendFile(res, "scripts/" + filename, "text/javascript"); // Specifies that we want to move out of the root/public directory into the scripts directory
}

//This function receives and sends all images folder .jpg files
void helperImages(response& res, string filename) {
	sendFile(res, "images/" + filename, "image/jpg"); // Specifies that we want to move out of the root/public directory into the images directory
}



int main()
{
	SimpleApp app; //This initializes our application named "app" which is an object of type SimpleApp from the crow library

	//Our default route. This should always return the Index.html file
	CROW_ROUTE(app, "/") // Default Route/Root directory
		([](const request &req, response &res) { // Doesn't need filename as a param because we'll never use a page other than Index.html for our default
			sendFile(res, "Index.html", "text/html"); // Index.html should ALWAYS be our default webpage and as such we know it's details enough to just send straight 
		});										  // -to the parent function, rather than the helper

	// Style (generaly .css) route
	CROW_ROUTE(app, "/styles/<string>")
		([](const request& req, response& res, string filename) {
			helperStyles(res, filename); //Pass to helper then "parent"
		});

	// Scripts (generally .js) route
	CROW_ROUTE(app, "/scripts/<string>")
		([](const request& req, response& res, string filename) {
			helperScripts(res, filename); //Pass to helper then "parent"
		});

	// Images (generally .jpg) route
	CROW_ROUTE(app, "/images/<string>")
		([](const request& req, response& res, string filename) {
			helperImages(res, filename); //Pass to helper then "parent"
		});



	// REQ5 Route
	CROW_ROUTE(app, "/<int>/<string>").methods(HTTPMethod::Post)
		([](const request& req, response& res, int a, string b) {
			ofstream fp;
			fp.open("../public/shoppingCart.txt", ios::app);
			if (fp) {
				fp << a << ", " << b << endl;
				fp.close();

				ifstream in("../public/Index.html", ifstream::in);
				ostringstream contents;
				contents << in.rdbuf();
				in.close();
				res.set_header("Content-Type", "text/html");
				res.write(contents.str());
				res.code = 200;
			} else {
				res.code = 404;
				res.write("Not Found");
			}
			res.end();
		});

	// REQ6 Route
	CROW_ROUTE(app, "/Checkout").methods(HTTPMethod::Post)
		([](const request& req, response& res) { // REQ6 a) handled in other route
			string uN = req.url_params.get("uN");
			string uP = req.url_params.get("uP");
			ifstream in("../public/Checkout.html", ifstream::in);
			if (in) {
				ostringstream contents;
				contents << in.rdbuf();
				in.close();
				res.set_header("Content-Type", "text/html");
				res.write(contents.str());
				
				// REQ6 e) done in HTML on form submission 

				// REQ6 b)
				if (uN == "admin" && uP == "abc123") { // REQ6 c)
					res.code = 202;
					// REQ6 f)
					ifstream cart("../public/shoppingCart.txt", ifstream::in);
					ostringstream shopCart;
					shopCart << cart.rdbuf();
					cart.close();
					res.write("\n\nShopping Cart:\n");
					res.write(shopCart.str());
				} else { // REQ6 d)
					res.code = 401;
					res.write("\n\n\n\nIncorrect Login Information.");
				}
			} else {
				res.code = 404;
				res.write("Not Found");
			}
			res.end();
		});

	CROW_ROUTE(app, "/Checkout.html").methods(HTTPMethod::Get)
		([](const request& req, response& res) { // REQ6 a)
			ifstream in("../public/Checkout.html", ifstream::in);
			if (in) {
				ostringstream contents;
				contents << in.rdbuf();
				in.close();
				res.set_header("Content-Type", "text/html");
				res.write(contents.str());
				res.code = 402;
			} else {
				res.code = 404;
				res.write("Not Found");
			}
			res.end();
		});
		
		
	/*
	 Routes required:		Completetion Status:
	 GET 					— Not Done (Part of Networking Lab10)
	 POST					— Not Done (Part of Networking Lab10)
	 PUT					— Not Done
	 PATCH					— Not Done
	 DELETE					— Not Done
	 OPTIONS				— Not Done
	*/
	
	//GET method Route
	/*CROW_ROUTE(app, "").methods(HTTPMethod::GET)
		([](const request& req, response& res) {
			//Do stuff
		}); */
		
	//POST method Route
	/*CROW_ROUTE(app, "").methods(HTTPMethod::POST)
		([](const request& req, response& res) {
			//Do stuff
		}); */
		
	//PUT method Route
	//A good usage of the PUT Method may be to "Create" the user accounts, since PUT is indempotent then calling it multiple 
		//-times will always produce the same result, where as calling POST multiple times will create the resource multiple times.
	/*CROW_ROUTE(app, "").methods(HTTPMethod::PUT)
		([](const request& req, response& res) {
			//Do stuff
		}); */
		
	//PATCH method Route
	//As I understand currently the idea is to update only part of the the user login resource 
		//(i.e. updating a users password but not username)
	/*CROW_ROUTE(app, "").methods(HTTPMethod::PATCH)
		([](const request& req, response& res) {
			//Do stuff
		}); */
		
	//DELETE method Route
	//A good usage of the DELETE Method may be to "reset" a users high score, whether this be an admin-only tool, 
		//-or available to all users is undetermined
	/*CROW_ROUTE(app, "").methods(HTTPMethod::DELETE)
		([](const request& req, response& res) {
			//Do stuff
		}); */
		
	//OPTIONS method Route
	//If my understanding is correct the technically this method should be allowed in addition to the others
	//E.g. this method shows what other methods are available, without acting on the resource, so maybe use it in conjunction
		// with the login page's routing?
	/*CROW_ROUTE(app, "").methods(HTTPMethod::OPTIONS)
		([](const request& req, response& res) {
			//Do stuff
		}); */


	// Generic HTML route
	CROW_ROUTE(app, "/<string>")
		([](const request& req, response& res, string filename) {
			helperHTML(res, filename); //Pass to helper then "parent"
		});
		
	app.port(23500).multithreaded().run(); //Here we get the app object to declare a port, that the site should run on multiple threads, and tell it to run!
	return 1;
}
