#include "socket_js.h"


client_js::client_js() {}

client_js::client_js(char *address) {
    unlink(address);
    this->SetSocketFile(address);
}


void client_js::SetSocketFile(char *address)/// 创建套接字
{
    if ((serv_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {

        throw "socket is error";
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, address);
    server_len = sizeof(struct sockaddr_un);

}

void client_js::connect()///接受链接 创建新的套接字
{
    client_len = sizeof(client_addr);
    connection = accept(serv_fd, (struct sockaddr *) &client_addr, &client_len);

}

bool client_js::js_listen()///监听套接字
{
    int x = listen(serv_fd, 20);
    if (x != 0) {
        throw "listen is error";
    }
}

bool client_js::js_bind()///绑定套接字
{
    if ((bind(serv_fd, (struct sockaddr *) &server_addr, server_len)) < 0)
        throw "bind is error";
}

int client_js::js_send(const char *message)///发送message
{
    return send(connection, message, strlen(message), 0);
}

int client_js::js_recv()///接收message
{
    memset(recv_temp, 0, 4096);
    return recv(connection, recv_temp, 4096, 0);
}

char *client_js::Get_message() {
    return recv_temp;
}

void client_js::js_close()///关闭套接字
{

    if ((close(this->connection) + close(this->serv_fd)) != 0) {
        throw "close is error";
    }

}



