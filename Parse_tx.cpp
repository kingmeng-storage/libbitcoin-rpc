#include "Parse_tx.h"


// str => int
unsigned int parse_hex(string x) {
    unsigned int result = 0;
    if (strcasecmp(x.data(), "ffffffff") > 0)
        throw "too long";
    for (int i = 0; i <= x.length() - 1; i++) {
        if (x[x.length() - i - 1] >= '0' && x[x.length() - i - 1] <= '9') {
            result += (x[x.length() - i - 1] - '0') * pow(16, i);
        } else if (x[x.length() - i - 1] >= 'a' && x[x.length() - i - 1] <= 'f') {
            result += (x[x.length() - i - 1] - 'a') * pow(16, i);
        } else if (x[x.length() - i - 1] >= 'A' && x[x.length() - i - 1] <= 'F') {
            result += (x[x.length() - i - 1] - 'A') * pow(16, i);
        }
    }
    return result;
}

string part_string(string &x, int length) {
    string part = "";
    std::copy_n(x.begin(), length, std::back_inserter(part));
    x.erase(0, length);
    return part;
}

string deserialize(string &x, int length) {
    string vvv = "";
    if (length <= 2) {
        return part_string(x, length);
    }


    for (int i = 0; i < length; i += 2) {
        vvv += x[i + 1];
        vvv += x[i];

    }
    reverse(vvv.begin(), vvv.end());
    x.erase(0, length);
    return vvv;
}


Json::Value parse(string x) {
    Json::Value root;
    Json::Value txin;
    Json::Value txout;
    root["version"] = parse_hex(deserialize(x, 8));
    int count = (parse_hex(deserialize(x, 2)));
    if (count < parse_hex("fd")) {
        root["in-counter"] = count;
    } else if (count == parse_hex("fd")) {
        root["in-counter"] = parse_hex(deserialize(x, 4));
    } else if (count == parse_hex("fe")) {
        root["in-counter"] = parse_hex(deserialize(x, 8));
    } else if (count == parse_hex("ff")) {
        root["in-counter"] = parse_hex(deserialize(x, 16));
    }
    Json::Value value;
    for (int i = 0; i < root["in-counter"].asInt(); ++i) {
        value["prv tx"] = deserialize(x, 64);
        value["Index"] = parse_hex(deserialize(x, 8));
        value["scriptSig"] = part_string(x, 2 * parse_hex(deserialize(x, 2)));
        value["sequence"] = parse_hex(deserialize(x, 8));
        txin.append(value);
    }
    count = parse_hex(deserialize(x, 2));
    if (count < parse_hex("fd")) {
        root["out-counter"] = count;
    } else if (count == parse_hex("fd")) {
        root["out-counter"] = parse_hex(deserialize(x, 4));
    } else if (count == parse_hex("fe")) {
        root["out-counter"] = parse_hex(deserialize(x, 8));
    } else if (count == parse_hex("ff")) {
        root["out-counter"] = parse_hex(deserialize(x, 16));
    }
    for (int j = 0; j < root["out-counter"].asInt(); ++j) {
        value.clear();
        value["amount"] = parse_hex(deserialize(x, 16));
        value["script"] = part_string(x, 2 * parse_hex(deserialize(x, 2)));
        value["Index"] = j;
        txout.append(value);

    }
    root["txin"] = txin;
    root["txout"] = txout;
    root["lock_time"] = parse_hex(deserialize(x, 8));
    return root;
}