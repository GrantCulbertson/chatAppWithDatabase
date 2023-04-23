//
//  namesAPI.cc - a microservice demo program
//
// Grant Culbertson + Samyak Shrestha
// Kenyon College, 2023
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <mariadb/conncpp.hpp>
#include <random>
#include "database.h"
#include "userEntry.h"
#include "httplib.h"
#include "nlohmann/json.hpp"
#include <sstream>

using json = nlohmann::json;
using namespace httplib;
using namespace std;

const int port = 5005;

//FUNCTION FOR GENERATING USER TOKENS (this is from ChatGPT, kind of cool)
std::string generate_token(int length) {
    std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string token;

    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate random characters from the charset
    std::uniform_int_distribution<int> dist(0, charset.size() - 1);
    for (int i = 0; i < length; i++) {
        token += charset[dist(gen)];
    }

    return token;
}


//Grab List of Usernames
string getUserList(map<string, string> const &activeUsers){
	string usernameList = "Users: ";
    for (auto const &pair: activeUsers) {
		usernameList += pair.first;
		usernameList += ", ";
    }
	usernameList.erase(usernameList.length() - 2);
	string jsonMessage = "{\"userList\":\""+usernameList+"\"}";
	return jsonMessage;
}

string getUserListMod(map<string, string> const &activeUsers){
	string usernameList;
    for (auto const &pair: activeUsers) {
		usernameList += pair.first;
		usernameList += ",";
    }
	usernameList.erase(usernameList.length() - 1);
	string jsonMessage = "{\"userList\":\""+usernameList+"\"}";
	return jsonMessage;
}


//Remove someone from the active users list
void removeUser(map<string, string> &activeUsers , string username, string token,map<string,string> &tokenMap){
	activeUsers.erase(username);
	tokenMap.erase(token);
}


//Add a message to a user.
void addMessage(string message, map<string,vector<string>> &messageMap,map<string,string> &tokenMap,string token) {
	/* iterate through users adding message to each */
	string username = tokenMap[token];
	string jsonMessage = "{\"user\":\""+username+"\",\"message\":\""+message+"\"}";
	for (auto userMessagePair : messageMap) {
		username = userMessagePair.first;
		messageMap[username].push_back(jsonMessage);
	}
}

//Show ... in the chat for people typing.
void getTypersList(map<string, string> const &typingMap , map<string,vector<string>> &messageMap , map<string,string> const &isTypingMap){
	string currentUser;
    for (auto const &pair: typingMap) {
		currentUser = pair.first;
    }
}
//Return list of people typing
string getTypersListMod(map<string, string> const &typingMap){
	string typerList;
    for (auto const &pair: typingMap) {
		typerList += pair.first;
		typerList += ",";
    }
	typerList.erase(typerList.length() - 1);
	string jsonMessage = "{\"typerList\":\""+typerList+"\"}";
	return jsonMessage;
}



//This function will add a user to a userMap, with their username, email, and password in a json string.
void addUser(string username, string password, string email, map<string,string> &userMap) {
	/* iterate through users adding message to each */
	string jsonMessage = "{\"user\":\""+username+"\",\"pass\":\""+password+"\",\"email\":\""+email+"\"}";
	userMap[username] = jsonMessage;
	cout << "addUser output: "<< userMap[username] << endl;
}

//Add someone to the active typers map
void addTyper(string username, map<string,string> &typingMap , map<string,string> &isTypingMap){
	string response = "yes";
	string message = "...";
	isTypingMap[username] = ".";
	typingMap[username] = "{\"user\":\""+username+"\",\"typing\":\""+response+"\"}";
}

//Remove someone from the active typer map.
void removeTyper(string username, map<string,string> &typingMap , map<string,string> &isTypingMap){
	string message = "";
	typingMap.erase(username);
	isTypingMap[username] = "{\"user\":\""+username+"\",\"message\":\""+message+"\"}";
}



string getMessagesJSON(string username, map<string,vector<string>> &messageMap) {
	/* retrieve json list of messages for this user */
	bool first = true;
	string result = "{\"messages\":[";
	for (string message :  messageMap[username]) {
		if (not first) result += ",";
		result += message;
		first = false;
	}
	result += "]}";
	messageMap[username].clear();
	return result;
}

//New Function (Database Additions)-----------------------------------------------------
string jsonResults(vector<contactEntry> pbList) {
	string res = "{\"results\":[";
	for (int i = 0; i<pbList.size(); i++) {
		res += pbList[i].json();
		if (i < pbList.size()-1) {
			res +=",";
		}
	}
	res += "]}";
	return res;
}

string jsonResultsMessages(vector<messageEntry> pbList) {
	string res = "{\"results\":[";
	for (int i = 0; i<pbList.size(); i++) {
		res += pbList[i].json();
		if (i < pbList.size()-1) {
			res +=",";
		}
	}
	res += "]}";
	return res;
}

//MAIN---------------------------------------------------------------------------------------
int main(void) {
  Server svr;
  int nextUser=0;
  map<string,vector<string>> messageMap;
  map<string,vector<string>> typingUsersMap;
  map<string,vector<string>> holdList;
  map<string,string> userMap;
  map<string,string> userEmail;
  map<string,string> activeUsers;
  map<string,string> typingMap;
  map<string,string> isTypingMap;
  map<string,string> tokenMap;


  /* "/" just returnsAPI name */
  //This is the API home page
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });

  //This is a test page
  svr.Get("/secretpage", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("You found the secret page 0_0", "text/plain");
  });

  


  //This is the part of the API that handles sending messages.
   // svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
    // res.set_header("Access-Control-Allow-Origin","*");
	// string username = req.matches[1];
	// string message = req.matches[2];
	// string result; 
    // if (!messageMap.count(username)) {
    	// result = "{\"status\":\"baduser\"}";
	// } else {
		// addMessage(username,message,messageMap);
		// result = "{\"status\":\"success\"}";
	// }
    // res.set_content(result, "text/json");
  // });
  
 
  //This part of the code grabs messages that are sent
   svr.Get(R"(/chat/fetch/(.*))", [&](const Request& req, Response& res) {
    string username = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
    string resultJSON = getMessagesJSON(username,messageMap);
    res.set_content(resultJSON, "text/json");
  });
  
    //This part of the code grabs a list of users
   svr.Get(R"(/chat/userlist)", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string result;
	result = getUserList(activeUsers);
	res.set_content(result, "text/json");
  });
  
   //This part of the code will remove a user from the active user list.
   svr.Get(R"(/chat/userlist/remove/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string token = req.matches[2];
	removeUser(activeUsers , username , token , tokenMap);
  });
  
  //This part of the code will update whether someone is typing.
     svr.Get(R"(/chat/typing/update/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	addTyper(username , typingMap , isTypingMap);
  });
  
   //This part of the code will remove someone from the typing map who is not typing.
     svr.Get(R"(/chat/typing/remove/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	removeTyper(username , typingMap , isTypingMap);
  });
 
    //This part of the code will return JSON of whether someone is typing
     svr.Get(R"(/chat/typing/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string result = typingMap[username];
	res.set_content(result, "text/json");
  });
  
    //This part of the code will send ... for someone typing. THIS IS DEPRECATED.
    svr.Get(R"(/chat/typingmessage/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string result = isTypingMap[username];
	res.set_content(result, "text/json");
  });
  
      //This part of the code will update the user list for the thing at the bottom of the web page
    svr.Get(R"(/chat/users)", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string result;
	result = getUserListMod(activeUsers);
	res.set_content(result, "text/json");
  });
  
    //This part of the code will update the user list for the thing at the bottom of the web page
    svr.Get(R"(/chat/users/typing)", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string result;
	result = getTypersListMod(typingMap);
	res.set_content(result, "text/json");
  });
  


//BELOW IS FOR DATABASE-----------------------------------------------------------------------------------
  contactDB ctdb; // Contact Book SQL Interface Object
  vector<contactEntry> results;
  vector<messageEntry> messageResults;
 
  //REGISTRATION: This Section should handle someone registering, /chat/register/username/email/password
  svr.Get(R"(/chat/register/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
	res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
	string email = req.matches[2];
	string password = req.matches[3];
    string result;
    vector<string> empty;
	//Check if user exists in the database already:
	results = ctdb.findByFirst(username);
	string jsonMessage = jsonResults(results);
	json json_obj = json::parse(jsonMessage);
	json getArray = json_obj["results"];
	string json_str = getArray.dump();
	bool emptyCheck = json_str == "[]";
	cout << "Registration is Running: " << jsonMessage << " " << json_str << " " << emptyCheck <<endl;
    if (json_str != "[]" or password.length() < 7){
    	result = "{\"status\":\"registrationfailure\"}";
    }else{
    	// Add users to various maps
    	messageMap[username]= empty;
		userEmail[username] = email;
    	result = "{\"status\":\"success\",\"user\":\"" + username + "\",\"email\":\"" + email + "\",\"pass\":\"" + password + "\"}";
		//Output some stuff
		cout << username << " registered" << endl;
		cout << "User Email: " << userEmail[username] << endl;
		//ADD USER TO PHPMYADMIN DATABASE
		ctdb.addUser(username,email,password);
    }

    res.set_content(result, "text/json");
  });
  
  //LOGGING IN-----------------------------------------------
  svr.Get(R"(/chat/join/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	//cout<< "this is log in output" << endl;
    string username = req.matches[1];
	string password = req.matches[2];
	string email = userEmail[username];
	//Get JSON object from SQL and parse to get Username and Password:
	results = ctdb.findByFirst(username);
	string jsonMessage = jsonResults(results);
	json json_obj = json::parse(jsonMessage);
	json getArray = json_obj["results"];
	json obtainedValues;
	//Go through the Array that comes with JSON object from SQL
    for (auto& values : getArray) {
        std::cout << values << std::endl;
		obtainedValues = values;
    }
	//Get the username and password from the array that is that JSON object.
	string gotUsername = obtainedValues["username"];
	string gotPassword = obtainedValues["password"];
	//Check that everything is working:
	// cout << "This is the Log in API Call" << jsonMessage << endl;
	// cout << "obtainedValues: " << obtainedValues << endl;
	// cout << "gotUsername: " << gotUsername << endl;
	// cout << "gotPassword: " << gotPassword << endl;
	string result;	
	
	if(gotUsername == username && gotPassword == password){
		activeUsers[username] = "this user is active";
		//Assign the user a random token.
		string token = generate_token(20);
		tokenMap[token] = username;
		cout << username << " joins, token is " << token << endl;
		result = "{\"status\":\"success\",\"user\":\"" + username + "\",\"token\":\"" + token + "\"}";
	}else{
		result = "{\"status\":\"failure\"}";
	}
    res.set_content(result, "text/json");
	getUserList(userMap);
  });
  
 
//ADD A MESSAGE TO THE DATABASE----------------------------------------------------------------------
   svr.Get(R"(/chat/send/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string message = req.matches[2];
	string token = req.matches[3];
	string result;
	//Grab information from the SQL database:
	//Get JSON object from SQL and parse to get Username and ID:
	results = ctdb.findByFirst(username);
	string jsonMessage = jsonResults(results);
	json json_obj = json::parse(jsonMessage);
	json getArray = json_obj["results"];
	json obtainedValues;
	
	//Go through the Array that comes with JSON object from SQL
    for (auto& values : getArray) {
        std::cout << values << std::endl;
		obtainedValues = values;
    }
	
	//Get the username and ID from the array that is that JSON object.
	string gotUsername = obtainedValues["username"];
	string gotID = obtainedValues["ID"];
	
	ctdb.addMessage(gotID,gotUsername,message);
    if (!tokenMap.count(token)) {
    	result = "{\"status\":\"baduser\"}";
	} else {
		addMessage(message,messageMap,tokenMap,token);
		result = "{\"status\":\"success\"}";
	}
    res.set_content(result, "text/json");
  });



//FUNCTION TO POPULATE CHAT HISTORY------------------------------------------------------------------
    svr.Get(R"(/chat/chatHistory)", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	//Fetch the SQL data and parse it.
	messageResults = ctdb.fetchMessages();
	string jsonMessage = jsonResultsMessages(messageResults);	
	//Break down the comma seperated JSON into an array. using for loop.
	json json_obj = json::parse(jsonMessage);
	json getArray = json_obj["results"];
	json obtainedValues;
	
	//Check output
	cout << "Messages have been fetched: " << jsonMessage << endl;
	
	string result;
	res.set_content(result, "text/json");
  });






  //What comes out in the Linux Console:
  cout << "Server listening on port " << port << endl;
  cout << "Chat Time" << endl;
  svr.listen("0.0.0.0", port);

}


