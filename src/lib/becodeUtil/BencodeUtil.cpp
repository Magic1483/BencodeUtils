#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>
#include <typeinfo>
#include <optional>
#include <tuple>
#include <bitset>


using json = nlohmann::json;

// std::string hexEncode(const std::string& input)
// {
//     static const char hexDigits[] = "0123456789ABCDEF";

//     std::string output;
//     output.reserve(input.length() * 2);
//     for (unsigned char c : input)
//     {
//         output.push_back('\\');
//         output.push_back('x');
//         output.push_back(hexDigits[c >> 4]);
//         output.push_back(hexDigits[c & 15]);
//     }
//     return output;
// }

std::string encode(json t){
    std::string res = "";

    if (t.is_number()){
        return res+="i"+t.dump()+"e";
    } 
    else if(t.is_string()){
        return std::to_string(t.get<std::string>().length())+":"+t.get<std::string>();
    }
    else if(t.is_array()){
        res+="l";
        for (json::iterator it = t.begin(); it != t.end(); ++it)
            res+=encode(it.value());
        return res+="e";
    }
    else if(t.is_object()){
        res+="d";
        for (json::iterator it = t.begin(); it != t.end();++it)
            res=res+encode(it.key())+encode(it.value());
        return res+="e";
    } else {
        throw std::runtime_error("Unknow type for encoding");
    }
}



/*
    RETURNS
    INT     "i52e"    --> 52                    integer <value> end
    STRING  "5:hello" --> "hello"               count_of_chars : <value>
    LIST    "li52e5:helloe" --> [52,"hello"]    list <values> end

    DECODE:
    INT STRING LIST DICT
*/
std::tuple<json,int> decode_bencoded_value(const std::string& encoded_value,bool is_bin_str = false) {
    // std::cout << "reminder  " << encoded_value << std::endl;
    if (std::isdigit(encoded_value[0])) {
        //  DECODE STRING
        std::cout << "decode string " << is_bin_str <<std::endl;
        size_t colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            int64_t number = std::atoll(number_string.c_str());
            std::string s = encoded_value.substr(colon_index + 1, number);

            std::cout << "string len   " << s.length() << " " <<std::endl;
            if (is_bin_str == true){
                
                std::vector<uint8_t> vec(s.begin(),s.end()); 
                std::cout << "decode bin    " << s << std::endl;    
                
                return std::make_tuple(vec,s.length()+number_string.length()+1);
            }

            return std::make_tuple(json(s),s.length()+number_string.length()+1);
        } else {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
    } 
    else if (encoded_value[0] == 'i'){
        //  DECODE INT
        // std::cout << "decode int" << "     "<< encoded_value <<std::endl;
        std::string res = "";
        for (int i = 1; encoded_value[i] != 'e' ; i++)
            res += encoded_value[i];
            
        return std::make_tuple(json(std::stoll(res)),res.length()+2);
    }
    else if (encoded_value[0] == 'l'){
        //  DECODE LIST
        // std::cout << "decode list" << "     " << encoded_value << std::endl;
        json list = json::array();
        int list_len = 2;
        std::string text  = encoded_value.substr(1);

        while (text[0] != 'e') {
            
            json res;
            int len;

            std::tie(res,len) = decode_bencoded_value(text);
            // std::cout << "return len " << len << std::endl;
            text = text.substr(len);
            
            list.push_back(res);
            list_len+=len;
        }
        
        // std::cout << " return list " << encoded_value << " " << text << "    list len is " << list_len <<std::endl;
        return std::make_tuple(list,list_len);
    }
    else if (encoded_value[0] == 'd'){
        //  DECODE DICT
        // std::cout << "decode dict" << "     "<< encoded_value << std::endl;
        json dict;
        int dict_len = 2;
        std::string text  = encoded_value.substr(1);

        while (text[0] != 'e') {
            json key,val;
            int len;

            std::tie(key,len) = decode_bencoded_value(text);
            text = text.substr(len);
            dict_len+=len;

            if (key == "pieces"){
                std::tie(val,len) = decode_bencoded_value(text,true);
            } else {
                std::tie(val,len) = decode_bencoded_value(text);
            }
            
            text = text.substr(len);
            dict_len+=len;

            // std::cout  << "key val  " << key << "   " << val << std::endl;
            dict[key] = val;

        }
        // std::cout << " return dict " << dict << " " << text <<std::endl;

        return std::make_tuple(dict,dict_len);
    }
    else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}
