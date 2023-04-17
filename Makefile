# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChatDatabase
# sudo chown ubuntu /var/www/html/restChatDatabase

all: PutHTML restChat

PutHTML:
	cp restChat.html /var/www/html/restChatDatabase/
	cp restChat.css /var/www/html/restChatDatabase/
	cp restChat.js /var/www/html/restChatDatabase/
	
	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/restChatDatabase/

restChat : restChat.cpp httplib.h
	$(CXX) -o restChat $(CXXFLAGS) restChat.cpp $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) $(BROTLI_SUPPORT) 

clean:
	rm restChat *.o