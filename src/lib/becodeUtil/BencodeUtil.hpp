#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>

#include "./lib/nlohmann/json.hpp"
#include "BencodeUtil.cpp"

using json = nlohmann::json;

// encode part
std::string encode(json t);


// decode part
std::tuple<json,int> decode_bencoded_value(const std::string& encoded_value,bool is_bin_str = false)

// tests