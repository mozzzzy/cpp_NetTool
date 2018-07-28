/*
 * name look up
 */
int nameLookup(std::string hostname, std::string service, struct addrinfo *&res);

/*
 * create tcp connection
 */
int connectServer(std::string hostname, std::string service);

/*
 * create ssl connection
 */
void connectSSL(int connectedSock, SSL *&ssl, SSL_CTX *&ctx);



/*
 * https get
 */
std::string httpsGet(std::string hostname, std::string port, std::string path, std::string query);
