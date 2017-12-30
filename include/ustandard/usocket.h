#ifndef __USOCKET_H__
#define __USOCKET_H__
__BEGIN_DECLS


int usocket_create_tcp_server(int port, int backlog);
int usocket_connect_to_tcp_server(const char* servername, int port);
int usocket_set_nonblocking(int fd);





















__END_DECLS
#endif /* usocket.h */
