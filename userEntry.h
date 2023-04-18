#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef CONTACTENTRY_H
#define CONTACTENTRY_H

class contactEntry {
public:
     contactEntry();
     contactEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p,sql::SQLString I);
     string text();
     string json();
     
	 //Stuff that can go in
     string first;
     string last;
     string phone;
     string type;
     string ID;
	 string address;
	 string age;
	 string username;
	 string email;
	 string password;

private:

};

#endif /* CONTACTENTRY_H */

