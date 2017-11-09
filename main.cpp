#include <iostream>
#include "socket_js.h"
#include "CZS_Require.h"


int main() {
    try {
        Js_deal CZS_TEST;
        client_js client("/tmp/0000.sock");
        client.js_bind();
        client.js_listen();
        client.connect();
        client.js_recv();
        Json::Reader reader;
        Json::Value root;
        reader.parse(client.Get_message(), root);
        CZS_TEST.set();
        CZS_TEST.select_command(root);
        client.js_send(CZS_TEST.result_root.toStyledString().data());
        client.js_close();
    }
    catch (const char *message) {
        std::cout << message << endl;
    }




    return 0;
}



