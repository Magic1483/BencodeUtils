#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>

#include "BencodeUtil.cpp"

using json = nlohmann::json;
using namespace std;


// encode part
std::string encode_string(std::string str);
std::string encode_int(int num);
std::string encode_list(json::array_t list);

// decode part
json decode_string(const std::string& encoded_value);
json decode_int(const std::string& encoded_value);
json decode_list(const std::string& encoded_value);
json decode_bencoded_value(const std::string& encoded_value);