#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef CONTACTENTRY_H
#define CONTACTENTRY_H

class contactEntry {
public:
     contactEntry();
     contactEntry(sql::SQLString f, sql::SQLString l, sql::SQLString p, sql::SQLString t,sql::SQLString I ,sql::SQLString d , sql::SQLString a);
     string text();
     string json();
     
     string first;
     string last;
     string phone;
     string type;
     string ID;
	 string address;
	 string age;

private:

};

#endif /* CONTACTENTRY_H */

