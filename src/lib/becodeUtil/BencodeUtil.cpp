#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>
#include <typeinfo>
#include <optional>


using json = nlohmann::json;


// ENCODING
std::string encode_string(std::string str){
    std::string res = "";
    return res+std::to_string(str.length())+":"+str;
}

std::string encode_int(int num){
    std::string res = "";
    return res+"i"+std::to_string(num)+"e";
}



std::string encode_list(json::array_t list){
    std::string res = "l";

    for (int i = 0; i < list.size(); i++)
        if (list[i].is_number()){
            res+=encode_int(list[i]);
        } else {
            res+=encode_string(list[i]);
        };

    return res+="e";
}



/*
DICT CAN CONTAINS ONLY
- string
- int
*/
std::string encode_dict(json dict){
    std::string res = "d";

    for (json::iterator it = dict.begin(); it != dict.end();++it){
        
        std::string key = encode_string(it.key());//string
        std::string val;

        // std::cout << it.value().is_number() << " " << key << std::endl;

        if (it.value().is_number()){
            val = encode_int(it.value());
        } else {
            val = encode_string(it.value());
        }
        res=res+key+val;
    }
        
    return res+="e";
}

// DECODING
std::tuple<json,int> decode_string(const std::string& encoded_value){
    if (std::isdigit(encoded_value[0])) {
        // Example: "5:hello" -> "hello"
        size_t colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            int64_t number = std::atoll(number_string.c_str());
            std::string str = encoded_value.substr(colon_index + 1, number);
            return std::make_tuple(json(str),str.length()+number_string.length()+1);
        } else {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
    }
}

std::tuple<json,int> decode_int(const std::string& encoded_value){
    if (encoded_value[0] == 'i'){
        std::string res = "";
        for (int i = 1; encoded_value[i] != 'e' ; i++)
            res += encoded_value[i];
            
        return std::make_tuple(json(std::stol(res)),res.length()+2);
    }
    else {
        throw std::runtime_error("Invalid encoded value: " + encoded_value);
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

        return decode_string(encoded_value);
    } 
    else if (encoded_value[0] == 'i'){
        return decode_int(encoded_value);
    }
    else if (encoded_value[0] == 'l'){
        std::string text  = encoded_value.substr(1);
        json list = json::array();
        
        

        while (text[0] != 'e')
        {   
            json res;
            int len = 0;

            std::tie(res,len) = decode_bencoded_value(text);

            // std::cout << list << std::endl; 
            list.push_back(res);
            text = text.substr(len);
        };
        
        return std::make_tuple(list,encoded_value.length()-2);
    }
    else if (encoded_value[0] == 'd'){
        std::string text  = encoded_value.substr(1);
        json dict = json::object();

        while (text[0] != 'e')
        {   
            json key,value;
            int len,len2 = 0;

            std::tie(key,len) = decode_bencoded_value(text);
            text = text.substr(len);//crop text
            
            std::tie(value,len2) = decode_bencoded_value(text);

            dict[key] = value;//insert value
            text = text.substr(len2);//2nd crop
            
            // std::cout << dict << "  " << text << "  len1  " << len << "len2  " << len2 << std::endl;
        };
        
        return std::make_tuple(dict,encoded_value.length()-2);
    }
    else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}