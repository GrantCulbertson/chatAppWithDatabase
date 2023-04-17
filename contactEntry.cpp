#include "userEntry.h"

contactEntry::contactEntry() {
}

contactEntry::contactEntry(sql::SQLString f, sql::SQLString l, sql::SQLString p, sql::SQLString t, sql::SQLString I,sql::SQLString d,sql::SQLString a) {
    first = f;
    last = l;
    phone = p;
    type = t;
    ID=I;
	address = d;
	age = a;
}

string contactEntry::text() {
	string result = ID + ". ";
	result += first + " ";
	result += last + " ";
	result += phone + " ";
	result += type + " ";
	result += address + " ";
	result += age;
	return result;

}

string contactEntry::json() {
	string result = "{\"ID\":\"" + ID + "\",";
	result += "\"first\":\"" + first + "\",";
	result += "\"last\":\"" + last + "\",";
	result += "\"phone\":\"" + phone + "\",";
	result += "\"type\":\"" + type + "\",";
	result += "\"address\":\"" + address + "\",";
	result += "\"age\":\"" + age + "\"}";
	return result;

}
