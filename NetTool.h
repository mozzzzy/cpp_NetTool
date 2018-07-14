/*
 * name look up
 */
int nameLookup(const char *hostname, const char *service, struct addrinfo *&res);

/*
 * create socket
 */
int connectServer(const char *hostname, const char *service);
