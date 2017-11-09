//
// Created by Kingmeng on 2017/11/7.
//

#ifndef LAMBDA_PARSE_TX_H
#define LAMBDA_PARSE_TX_H

#include <iostream>
#include <json/json.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;



unsigned int parse_hex(string x);

string part_string(string &x, int length);

string deserialize(string &x, int length);


Json::Value parse(string x);


#endif //LAMBDA_PARSE_TX_H
