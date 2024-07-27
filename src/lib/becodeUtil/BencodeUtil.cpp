#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>
#include <typeinfo>
#include <optional>
#include <tuple>


using json = nlohmann::json;


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
std::tuple<json,int> decode_bencoded_value(const std::string& encoded_value) {
    if (std::isdigit(encoded_value[0])) {
        //  DECODE STRING
        std::cout << "decode string" << std::endl;
        size_t colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            int64_t number = std::stoll(number_string);
            std::string str = encoded_value.substr(colon_index + 1, number);

            // std::cout << str.length()  << " " << str <<std::endl;
            return std::make_tuple(json(str),str.length()+number_string.length()+1);
        } else {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
    } 
    else if (encoded_value[0] == 'i'){
        //  DECODE INT
        std::cout << "decode int" << "     "<< encoded_value <<std::endl;
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
            std::cout << "return len " << len << std::endl;
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

            std::tie(val,len) = decode_bencoded_value(text);
            text = text.substr(len);
            dict_len+=len;

            dict[key] = val;

        }
        // std::cout << " return dict " << dict << " " << text <<std::endl;

        return std::make_tuple(dict,dict_len);
    }
    else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}
