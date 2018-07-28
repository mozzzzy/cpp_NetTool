/*
 * 2018/07/14
 */
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>


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
 * create tcp connection
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


/*
 * create ssl connection
 */
void connectSSL(int connectedSock, SSL *&ssl, SSL_CTX *&ctx){

	/* create SSL connection */
	SSL_library_init();
	ctx = SSL_CTX_new(SSLv23_client_method());

	ssl = SSL_new(ctx);
	SSL_set_options(ssl, SSL_OP_NO_TICKET);
	SSL_set_fd(ssl, connectedSock);

	SSL_connect(ssl);

}


/*
 * send https GET
 */
void httpsGet(SSL *&ssl, std::string hostname, std::string port, std::string path, std::string query, std::string &response){


	// create write message
	std::string write_buf = "GET " + path;
	if(query != ""){
		write_buf += "?" + query;
	}
	write_buf += " HTTP/1.1\r\n";
	write_buf += "Host: " + hostname + "\r\n" + "User-Agent: NetTool\r\n" + "Accept: */*\r\n\r\n";

	// send message
	SSL_write(ssl, write_buf.c_str(), strlen(write_buf.c_str()));

	/* read response */
	response = "";
	char tmp_read_buf[1024];
	int read_size = 0;
    while( (read_size = SSL_read(ssl, tmp_read_buf, 1024)) ){
		response += tmp_read_buf;
    }

}







/*
 * send https GET only this function
 */
std::string instantHttpsGet(std::string hostname, std::string port, std::string path, std::string query){

	// create tcp connection
	int connectedSock = connectServer(hostname, port);

	// create SSL connection
	SSL_CTX *ctx;
	SSL *ssl;
	connectSSL(connectedSock, ssl, ctx);

    // create write message
	std::string write_buf = "GET " + path;
	if(query != ""){
		write_buf += "?" + query;
	}
	write_buf += " HTTP/1.1\r\n";
	write_buf += "Host: " + hostname + "\r\n" + "User-Agent: NetTool\r\n" + "Accept: */*\r\n\r\n";

	// send message
    SSL_write(ssl, write_buf.c_str(), strlen(write_buf.c_str()));


    /* read response */
	std::string read_buf;

    char tmp_read_buf[1024];
    int read_size = 0;
    while( (read_size = SSL_read(ssl, tmp_read_buf, 1024)) ){
		read_buf += tmp_read_buf;
    }


    SSL_shutdown(ssl);
    SSL_free(ssl);

    /*close socket*/
    close(connectedSock);
    SSL_CTX_free(ctx);

	return read_buf;
}


/*
 * https POST
 */
//std::string httpsPost(std::string hostname, std::string port, std::string path, std::string query){
//}


/*
 * https PUT
 */
//std::string httpsPut(std::string hostname, std::string port, std::string path, std::string query){
//}


#ifdef DEBUG
int main(void){

	std::string hostname = "www.workspace01gl.net";
	std::string service = "10443";
	struct addrinfo *res;

	/*
	 * name lookup test
	 */
	int result = nameLookup(hostname, service, res);
	std::cout << "result --> " << result << std::endl;

	// print ip address
	struct in_addr addr;
	char buf[16];
	addr.s_addr = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.s_addr;
	inet_ntop(AF_INET, &addr, buf, sizeof(buf));
	std::cout << "ip address : " << buf << std::endl;



	/*
     * name lookup and connect server and send httpsGet test
	 */
	// connect server
	int connectedSock = connectServer(hostname, service);
	std::cout << "connectedSock = " << connectedSock << std::endl;

	// connect ssl
    SSL_CTX *ctx;
    SSL *ssl;
    connectSSL(connectedSock, ssl, ctx);
	std::cout << "connect ssl" << std::endl;


	// send https get
	std::string https_response = "";
	std::string https_path = "/";
	std::string https_query = "";
	httpsGet(ssl, hostname, service, https_path, https_query, https_response);
	std::cout << "response : " << https_response << std::endl;

	/*close ssl connection*/
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    /*close socket*/
    close(connectedSock);


	/*
	 * https Get test
	 */
	std::string response = instantHttpsGet("www.workspace01gl.net", "10443", "/", "");
	std::cout << "response : " << response;



	return 0;
}
#endif
