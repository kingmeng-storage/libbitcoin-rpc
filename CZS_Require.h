//
// Created by Kingmeng on 2017/10/27.
//

#ifndef LIBBITCOIN_JS_CZS_REQUIRE_H
#define LIBBITCOIN_JS_CZS_REQUIRE_H

#include <bitcoin/client.hpp>
#include <functional>
#include "Parse_tx.h"
#include <memory>
#include <map>

using namespace bc::client;
using namespace bc;
using namespace bc::wallet;
using namespace bc::chain;

class Js_deal {
public:
    obelisk_client *proxy_client;
    config::endpoint *endpoint;
    chain::transaction *tx;

    Json::Value transaction_fetch_transaction(const Json::Value &src);

    Json::Value transaction_pool_broadcast(const Json::Value &src);

    Json::Value blockchain_fetch_last_height(const Json::Value &src);

    Json::Value blockchain_fetch_transaction_index(const Json::Value &src);

    Json::Value blockchain_fetch_unspent_outputs(const Json::Value &src);

    void select_command(Json::Value &src);

    void set();

    ~Js_deal();

    void creator_hash_digest(const char *data);

    Json::Value result_root;
    typedef Json::Value (Js_deal::*func)(const Json::Value&);
    std::map<string,func> m_func;

private:
    std::shared_ptr<points_value> test_points_value;
    std::shared_ptr<payment_address> address;
    std::shared_ptr<hash_digest> hash;
    void func_prase();

};


#endif //LIBBITCOIN_JS_CZS_REQUIRE_H
