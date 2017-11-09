#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <zconf.h>
#include <sys/ioctl.h>
#include <iostream>
#include <unistd.h>
#include <cerrno>

class client_js {
public:
    client_js();

    explicit client_js(char *address);

    void connect();

    void SetSocketFile(char *address);

    bool js_listen();

    bool js_bind();

    int js_send(const char *message);

    int js_recv();

    void js_close();

    char *Get_message();

private:
    char recv_temp[4096];
    int serv_fd;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;
    socklen_t server_len;
    socklen_t client_len;
    int connection;

};

