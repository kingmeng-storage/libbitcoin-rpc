//
// Created by Kingmeng on 2017/10/27.
//

#include "CZS_Require.h"


using namespace std::placeholders;

void Js_deal::set()///设置网络接入点
{
    this->endpoint = new config::endpoint("tcp://10.10.0.233", 8091);
    this->proxy_client = new obelisk_client(2000, 0);
    this->proxy_client->connect(*endpoint);
    delete this->endpoint;
    this->endpoint = nullptr;

}

void Js_deal::func_prase()
{
    m_func["transaction_fetch_transaction"]      = &Js_deal::transaction_fetch_transaction;
    m_func["transaction_pool_broadcast"]         = &Js_deal::transaction_pool_broadcast;
    m_func["blockchain_fetch_last_height"]       = &Js_deal::blockchain_fetch_last_height;
    m_func["blockchain_fetch_transaction_index"] = &Js_deal::blockchain_fetch_transaction_index;
    m_func["blockchain_fetch_unspent_outputs"]   = &Js_deal::blockchain_fetch_unspent_outputs;
}






void Js_deal::select_command(Json::Value &src)///命令选择
{
    func_prase();

        if(m_func.find(src["command"].asString())==m_func.end())
        {
            throw "command not found";
        }

    {
        result_root=(this->*m_func[src["command"].asString()])(src);
    }
}



void Js_deal::creator_hash_digest(const char* data)
{
    data_chunk temp;
    decode_base16(temp, data);
    this->hash = std::make_shared<hash_digest>();

    memcpy(this->hash->begin(), temp.data(), 32);
    std::reverse(hash->begin(), hash->end());
}


/*---------------------------COMMAND---------------------------*/
 Json::Value Js_deal::transaction_fetch_transaction(const Json::Value &src) {
    Json::Value root;

    this->creator_hash_digest(src["data"].asString().data());
    proxy_client->blockchain_fetch_transaction([](const code &err) {
        std::cerr << err.message() << std::endl;
    }, [this, &root](chain::transaction handler) {
        std::string tx_str = encode_base16(handler.to_data());
        root = parse(tx_str);
    }, *this->hash);
    proxy_client->wait();
    return root;
}


Json::Value Js_deal::transaction_pool_broadcast(const Json::Value &src) {
    Json::Value root;
    const auto callback = [&root](const code &err) { root["Result"] = err.message(); };
    chain::transaction tx;
    data_chunk chunk;
    decode_base16(chunk, src["data"].asString().data());
    tx.from_data(chunk);
    this->proxy_client->transaction_pool_broadcast(callback, callback, tx);
    this->proxy_client->wait();
    return root;
}


Json::Value Js_deal::blockchain_fetch_last_height(const Json::Value &src) {

    Json::Value root;
    const auto on_reply = [&root](const size_t &result) { root["last_height"] = (int) result; };
    const auto on_error = [](const code &err) { std::cout << err.message() << std::endl; };
    this->proxy_client->blockchain_fetch_last_height(on_error, on_reply);
    this->proxy_client->wait();
    return root;
}

Json::Value Js_deal::blockchain_fetch_transaction_index(const Json::Value &src) {
    Json::Value root;
    const auto on_error = [&root](const code &err) { root["Result"] = err.message(); };
    const auto on_reply = [this, &root](const size_t &a, const size_t &b) {
        root["block_heigth"] = (int) a;
        root["block_index"] = (int) b;
    };
    this->creator_hash_digest(src["DATA"].asString().data());
    this->proxy_client->blockchain_fetch_transaction_index(on_error, on_reply, *this->hash);
    proxy_client->wait();
    return root;

}

Json::Value Js_deal::blockchain_fetch_unspent_outputs(const Json::Value &src) {
    using namespace libbitcoin::wallet;
    Json::Value root;
    const auto on_error = [&root](const code &err) { root["result"]=err.message().data(); };
    const auto on_reply = [this](const chain::points_value &p) {
        this->test_points_value =std::make_shared<points_value>(p);
    };
    this->address = std::make_shared<payment_address>(src["data"].asString().data());
    this->proxy_client->blockchain_fetch_unspent_outputs(on_error, on_reply, *this->address, src["amount"].asInt(), select_outputs::algorithm::individual);
    proxy_client->wait();
    return root;
}

Js_deal::~Js_deal()
{

}
















