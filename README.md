# restChatCpp
A REST based web chat program using C++
## Uses cpp-httplib for the microservice
 - ```https://github.com/yhirose/cpp-httplib```
 
## Install SSL and compression services
 - ```sudo apt-get install libssl-dev```
 - ```sudo apt-get install libz-dev```

## MariaDB
 - This assumes MariaDB is installed. If not folllow these instructions: https://www.digitalocean.com/community/tutorials/how-to-install-mariadb-on-ubuntu-22-04
 - Install the MariaDB connector: https://mariadb.com/docs/skysql/connect/programming-languages/cpp/install/
 
## Set Javascript IP address to your VM address
 - Edit ```restChat.js``` so that ```baseUrl``` is your VM's IP address

## Set up App
 - ```sudo mkdir /var/www/html/restChatDatabase/```
 - ```sudo chown ubuntu /var/www/html/restChatDatabase/```

## Make and run
 - ```make```
 - ```./restChat```

## Go to your VM URL/namesCpp/namelookup.html
# chatApp

##Grant Mirror:
## http://18.116.8.156/restChat/restChat.html#
## Sammy Mirror:
http://13.58.143.75/restChat/restChat.html
# chatAppWithDatabase
