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
