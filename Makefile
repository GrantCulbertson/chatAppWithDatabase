# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChatDatabase
# sudo chown ubuntu /var/www/html/restChatDatabase

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++14  -Wno-deprecated-declarations

RM= /bin/rm -f

all: PutHTML restChat

PutHTML:
	cp restChat.html /var/www/html/restChatDatabase/
	cp restChat.css /var/www/html/restChatDatabase/
	cp restChat.js /var/www/html/restChatDatabase/
	
	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/restChatDatabase/

contactEntry.o: contactEntry.cpp userEntry.h
	$(CC) -c $(CFLAGS) contactEntry.cpp

contactDB.o: contactDB.cpp database.h
	$(CC) -c $(CFLAGS) -I/usr/include/cppconn contactDB.cpp

restChat.o: restChat.cpp httplib.h nlohmann/json.hpp
	$(CC) -c $(CFLAGS) restChat.cpp


restChat : restChat.o contactDB.o contactEntry.o
	$(CC) restChat.o contactDB.o contactEntry.o -o restChat -L/usr/local/lib -lmariadbcpp
	#$(CXX) -o restChat $(CXXFLAGS) restChat.cpp $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) $(BROTLI_SUPPORT) -L/usr/local/lib -lmariadbcpp

clean:
	rm restChat *.o