#include "userEntry.h"

contactEntry::contactEntry() {
}

contactEntry::contactEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p,sql::SQLString I) {
    username = u;
    email = e;
    password = p;
    ID=I;
}



string contactEntry::text() {
	string result = ID + ". ";
	result += username + " ";
	result += email + " ";
	result += password;
	return result;

}

string contactEntry::json() {
	string result = "{\"ID\":\"" + ID + "\",";
	result += "\"username\":\"" + username + "\",";
	result += "\"password\":\"" + password + "\",";
	result += "\"email\":\"" + email + "\"}";
	return result;

}

//Mesage Entry class: 
messageEntry::messageEntry() {
}

messageEntry::messageEntry(sql::SQLString u, sql::SQLString m, sql::SQLString t,sql::SQLString I) {
    username = u;
    message = m;
    timestamp = t;
    ID=I;
}



string messageEntry::text() {
	string result = ID + ". ";
	result += username + " ";
	result += message + " ";
	result += timestamp;
	return result;

}

string messageEntry::json() {
	string result = "{\"ID\":\"" + ID + "\",";
	result += "\"username\":\"" + username + "\",";
	result += "\"message\":\"" + message + "\",";
	result += "\"timestamp\":\"" + timestamp + "\"}";
	return result;

}
