#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/usocket.h"


int usocket_create_tcp_server(int port, int backlog)
{
    int serverfd;
    int ret;
    struct sockaddr_in servaddr;
                      
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0) {
        ulogerr("socket error(%s).\n", strerror(errno));
        return serverfd;
    }

    ulogdbg("serverfd = %d\n", serverfd);
    int val = 1;
    ret = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(0 != ret) {
        ulogerr("set SO_REUSEADDR error: %s(errno: %d)\n",strerror(errno),errno);
        close(serverfd);
        serverfd = -1;
        return serverfd;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    ret = bind(serverfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(0 != ret) {
        ulogerr("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        close(serverfd);
        serverfd = -1;
        return serverfd;
    }

    ret = listen(serverfd, backlog);
    if(0 != ret) {
        ulogerr("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        close(serverfd);
        serverfd = -1;
        return serverfd;
    }

    return serverfd;
}


#include <netdb.h>
int usocket_connect_to_tcp_server(const char* servername, int port)
{
    int clientfd;
    struct sockaddr_in servaddr;

    struct hostent* host = gethostbyname(servername);
    if(NULL == host) {
        ulogerr("gethostbyname (%s) error: %s(errno: %d)\n", servername, strerror(errno),errno);
        return -1;
    }

    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0) {
        ulogerr("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return clientfd;
    }

    bzero(&servaddr,sizeof(servaddr)); 
    servaddr.sin_family=AF_INET; 
    servaddr.sin_port=htons(port); 
    servaddr.sin_addr=*((struct in_addr *)host->h_addr); 

#if 0
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if( inet_pton(AF_INET, servername, &servaddr.sin_addr) <= 0){
        ulogerr("inet_pton error for %s\n", servername);
        close(clientfd);
        clientfd = -1;
        return clientfd;
    }
#endif

    if( connect(clientfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        ulogerr("connect error: %s(errno: %d)\n",strerror(errno),errno);
        close(clientfd);
        clientfd = -1;
        return clientfd;
    }

    return clientfd;
}


int usocket_set_nonblocking(int fd)
{
    int ret = 0;

    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }

    if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }

    return ret;
}

