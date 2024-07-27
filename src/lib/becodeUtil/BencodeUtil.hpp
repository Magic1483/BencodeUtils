#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>

#include "BencodeUtil.cpp"
#include "test.cpp"

using json = nlohmann::json;
using namespace std;


// encode part
std::string encode_string(std::string str);
std::string encode_int(int num);
std::string encode_list(json::array_t list);
std::string encode_dict(json dict);

// decode part
std::tuple<json,int> decode_string(const std::string& encoded_value);
std::tuple<json,int> decode_int(const std::string& encoded_value);
std::tuple<json,int> decode_list(const std::string& encoded_value);
std::tuple<json,int> decode_bencoded_value(const std::string& encoded_value);

// tests
void BencodeTest();