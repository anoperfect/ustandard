#include "ustandard/ustandard.h"
#include "sys/epoll.h"




const char* kcmds = "create server #port(e.g. create server 8080)\n"
    "connect ip:port\n"

    "\n"
;


#define response(fmt...) {printf("\e[34;1m  "); printf(fmt); printf("\e[0m");}

#define STRING_CREATE_SERVER    "create server "
#define STRING_ACCEPT           "accept"
#define STRING_CONNECT          "connect "
#define STRING_SEND_FILE        "sendfile "

struct socket_cfg {
    int eventfd;
    bool server_mode;
    long port;
    int serverfd;
    int clientfd;
    int pipe_cmd[2];
    pthread_t pth;

    void* buf_receive[1024];
    int nbuf_receive;



}kcfg = {0};


int create_server(void);
int fd_monitor_read(int fd);
int read_client(void);
int printf_receive(void);


int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);
    int ret = 0; ret = ret;

    printf("%s", kcmds);

    kcfg.eventfd = epoll_create(1024);
    assert(kcfg.eventfd > 0);

    int nread;
    fd_monitor_read(0);

    char cmd[1024];
    while(1) {
        printf("> ");

        struct epoll_event events[1024];
        int nevents = epoll_wait(kcfg.eventfd, events, 1024, -1);
        assert(nevents > 0);

        int idx;
        for(idx=0; idx<nevents; idx++) {
            int fd_on = events[idx].data.fd;
            if(0 == fd_on) {
                nread = read(0, cmd, sizeof(cmd)-1);
                assert(nread > 0);
                cmd[nread] = '\0';
                ustrcut_char_last(cmd, '\n');
                printf("  get cmd : [%s]\n", cmd);

                if(0 == strncmp(cmd, STRING_CREATE_SERVER, strlen(STRING_CREATE_SERVER))) {
                    kcfg.port = atol(cmd+strlen(STRING_CREATE_SERVER));

                    create_server();


                    ret = pipe(kcfg.pipe_cmd);
                    assert(kcfg.server_mode);
                    assert(0 == ret);

                    fd_monitor_read(kcfg.serverfd);
                }
                else if(0 == strncmp(cmd, STRING_ACCEPT, strlen(STRING_ACCEPT))) {
                    assert(kcfg.server_mode);
                    assert(kcfg.pipe_cmd[0] > 0 && kcfg.pipe_cmd[1] > 0);
                    write(kcfg.pipe_cmd[1], STRING_ACCEPT, strlen(STRING_ACCEPT));




                }
                else if(0 == strncmp(cmd, STRING_CONNECT, strlen(STRING_CONNECT))) {
                    char url[10240];
                    um_strcpy(url, sizeof(url), cmd+strlen(STRING_CONNECT));
                    char* port = strchr(url, ':');
                    assert(port);

                    kcfg.port = atol(port+1);
                    *port = '\0';

                    kcfg.clientfd = usocket_connect_to_tcp_server(url, kcfg.port);
                    assert(kcfg.clientfd > 0);

                    response("connected to %s port %ld, fd %d.\n", url, kcfg.port, kcfg.clientfd);


                    fd_monitor_read(kcfg.clientfd);




                }
                else if(0 == strncmp(cmd, STRING_SEND_FILE, strlen(STRING_SEND_FILE))) {
                    char url[10240];
                    um_strcpy(url, sizeof(url), cmd+strlen(STRING_SEND_FILE));

                    char buf[10240];
                    size_t size = ufile_read(url, buf, sizeof(buf));
                    assert(size > 0 && size < sizeof(buf));

                    ssize_t nwrite = write(kcfg.clientfd, buf, size);
                    assert(nwrite == size);

                }
                else {
                    response("unkown command 0.\n");


                }
            }
            else if(kcfg.serverfd == events[idx].data.fd) {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len;
                char client_ip_str[1024];


                client_addr_len = sizeof(client_addr);
                kcfg.clientfd = accept(kcfg.serverfd, (struct sockaddr *) &client_addr, &client_addr_len);
                assert (kcfg.clientfd > 0);

                const char* tmp = inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip_str, sizeof(client_ip_str));
                if(tmp) {
                    printf("accept a client[%d] from: %s\n", kcfg.clientfd, client_ip_str);
                }
                else {
                    ulogerr("accept a client[%d] but inet_ntop failed.\n", kcfg.clientfd);
                }

                fd_monitor_read(kcfg.clientfd);
            }
            else if(kcfg.clientfd == events[idx].data.fd) {

#if 0
                char buf[10240];
                int nread = read(kcfg.clientfd, buf, sizeof(buf)-1);
                printf("nread = %d, %s\n", nread, strerror(errno));
                assert(nread >= 0);
                ufile_write("websocket.request.txt", "w", buf, nread);

                buf[nread] = '\0';

                //printf("%s\n", buf);

                ustr_t str;
                ustr_replaces(buf, "\r\n", "\\r\\n\r\n", &str);
                //printf("%s\n", str.s);
                um_strcpy(buf, sizeof(buf), str.s);
                um_free(str.s);

                printf("%s\n", buf);

                void** pp = &kcfg.buf_receive[kcfg.nbuf_receive];
                kcfg.nbuf_receive ++;

                *pp = um_malloc(nread);
                memcpy(*pp, buf, nread);
#endif
                read_client();
                printf_receive();

            }

        }
    }

    return 0;
}


int create_server(void)
{
    int ret = 0;

    long port = (long)kcfg.port;
    kcfg.serverfd = usocket_create_tcp_server(port, 20);
    response("create server on port %ld, fd %d.\n", port, kcfg.serverfd);
    assert(kcfg.serverfd > 0);
    kcfg.server_mode = true;

    return ret;
}


int fd_monitor_read(int fd)
{
    int ret = 0;

    usocket_set_nonblocking(fd);

    struct epoll_event event_add;
    event_add.events = EPOLLIN | EPOLLET;
    event_add.data.fd = fd;
    ret = epoll_ctl(kcfg.eventfd, EPOLL_CTL_ADD, fd, &event_add);
    if(0 != ret) {
        ulogerr("epoll_ctl EPOLL_CTL_ADD fd %d op %u (%s).\n", fd, event_add.events, strerror(errno));
        assert(0);
    }

    return ret;
}


int read_client(void)
{
    int ret = 0;

    char* buf;
    size_t size = 0;
    size_t total = 10240;

    buf = um_malloc(total);

    while(1) {
        assert(total >= size);
        if(total == size) {
            total += 10240;
            buf = um_realloc(buf, total);
        }

        int nread = read(kcfg.clientfd, buf+size, total-size);
        if(nread <= 0) {
            printf("nread = %d, %s\n", nread, strerror(errno));
            break;
        }
        printf("nread = %d.\n", nread);
        size += nread;

        /*
        ufile_write("websocket.request.txt", "w", buf, nread);
        //buf[nread] = '\0';
        //printf("%s\n", buf);

        ustr_t str;
        ustr_replaces(buf, "\r\n", "\\r\\n\r\n", &str);
        //printf("%s\n", str.s);
        um_strcpy(buf, sizeof(buf), str.s);
        um_free(str.s);

        printf("%s\n", buf);

        void** pp = &kcfg.buf_receive[kcfg.nbuf_receive];
        kcfg.nbuf_receive ++;

        *pp = um_malloc(nread);
        memcpy(*pp, buf, nread);
        */
    }

    buf[size] = '\0';

    assert(size >= 0);
    
    kcfg.buf_receive[kcfg.nbuf_receive] = buf;
    kcfg.nbuf_receive ++;

    uloginf("\n");
    char name[1024];
    snprintf(name, sizeof(name), "%02d.recv", kcfg.nbuf_receive);
    ufile_write(name, "w", buf, size);
    uloginf("\n");


    return ret;
}


int printf_receive(void)
{
    int ret = 0;

    printf("%s\n", (char*)kcfg.buf_receive[kcfg.nbuf_receive-1]);
    
    


    return ret;
}





/*
                    
                    ret = pthread_create(&kcfg.pth, NULL, func_create_server, (void*)port);
                    assert(0 == ret);
                   
void* func_create_server(void* arg);
void* func_create_server(void* arg)
{
    long port = (long)kcfg.port;
    kcfg.serverfd = usocket_create_tcp_server(port, 20);
    response("create server on port %ld, fd %d.\n", port, kcfg.serverfd);
    assert(kcfg.serverfd > 0);

    int nread;
    
    while(1) {
        char cmd[1024];
        nread = read(kcfg.pipe_cmd[0], cmd, sizeof(cmd)-1);
        assert(nread > 0);

        if(0 == strcmp(cmd, "accept")) {
            struct sockaddr_in client_addr;
            socklen_t client_addr_len;
            char client_ip_str[1024];


            client_addr_len = sizeof(client_addr);
            kcfg.clientfd = accept(kcfg.serverfd, (struct sockaddr *) &client_addr, &client_addr_len);
            assert (kcfg.clientfd > 0);

            const char* tmp = inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip_str, sizeof(client_ip_str));
            if(tmp) {
                printf("accept a client[%d] from: %s\n", kcfg.clientfd, client_ip_str);
            }
            else {
                ulogerr("accept a client[%d] but inet_ntop failed.\n", kcfg.clientfd);
            }
        }
        else if(0 == strcmp(cmd, "read")) {

            char buf[1024];
            while(1) {
                int nread = read(kcfg.clientfd, buf, sizeof(buf)-1);
                printf("nread = %d, %s\n", nread, strerror(errno));
                assert(nread >= 0);
                buf[nread] = '\0';

                printf("%s\n", buf);

                ustr_t str;
                ustr_replaces(buf, "\r\n", "\\r\\n\r\n", &str);
                printf("%s\n", str.s);
                um_strcpy(buf, sizeof(buf), str.s);

                printf("%s\n", buf);
            }
        }
        else {
            response("unkown cmd.\n");
        }
    }


    return NULL;
}

*/









