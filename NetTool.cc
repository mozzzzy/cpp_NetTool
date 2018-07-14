/*
 * 2018/07/14
 */
#include<iostream>
#include<cstring>
#include<netdb.h>
#include<arpa/inet.h>
#include <unistd.h>

/*
 * name look up
 */
int nameLookup(std::string hostname, std::string service, struct addrinfo *&res){

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	int err;
	if((err = getaddrinfo(hostname.c_str(), service.c_str(), &hints, &res)) != 0) {
		return err;
	}

	return 0;
}

/*
 * create socket
 */
int connectServer(std::string hostname, std::string service){

	int sock = -1;
	struct addrinfo *res, *resc;

	// name lookup
	int err = nameLookup(hostname, service, res);

	for (resc=res; resc!=NULL; resc=resc->ai_next) {
		// get sock file descriptor
		sock = socket(resc->ai_family, resc->ai_socktype, resc->ai_protocol);
		if (sock < 0) {
			continue;
		}

		// connect to server
        	// if couldn't connect to server
		if (connect(sock, resc->ai_addr, resc->ai_addrlen) != 0) {
			close(sock);
			continue;

		// else if connect to server succesfully
		}else{
			break;
		}	
	}

	freeaddrinfo(res);

	if(resc == NULL) {
		// couldn't connect to server
		return -1;
	}

	return sock;
}


#ifdef DEBUG
int main(void){

	//const char *hostname = "www.google.com";
	std::string hostname = "www.google.com";
	//const char *service = "80";
	std::string service = "80";
	struct addrinfo *res;

	// name lookup
	int result = nameLookup(hostname, service, res);
	std::cout << "result --> " << result << std::endl;

	// print ip address
	struct in_addr addr;
	char buf[16];
	addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
	inet_ntop(AF_INET, &addr, buf, sizeof(buf));
	std::cout << "ip address : " << buf << std::endl;


	// name lookup and connect to server
	int connectedSock = connectServer(hostname, service);
	std::cout << "connectedSock = " << connectedSock << std::endl;

	// write

	// listen

	// close
	close(connectedSock);


	return 0;
}
#endif
