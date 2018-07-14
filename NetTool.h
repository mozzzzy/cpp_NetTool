/*
 * name look up
 */
int nameLookup(std::string hostname, std::string service, struct addrinfo *&res);

/*
 * create socket
 */
int connectServer(std::string hostname, std::string service);
