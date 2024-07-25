#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>
#include <typeinfo>


using json = nlohmann::json;

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


json decode_string(const std::string& encoded_value){
    if (std::isdigit(encoded_value[0])) {
        // Example: "5:hello" -> "hello"
        size_t colon_index = encoded_value.find(':');
        if (colon_index != std::string::npos) {
            std::string number_string = encoded_value.substr(0, colon_index);
            int64_t number = std::atoll(number_string.c_str());
            std::string str = encoded_value.substr(colon_index + 1, number);
            return json(str);
        } else {
            throw std::runtime_error("Invalid encoded value: " + encoded_value);
        }
    }
}

json decode_int(const std::string& encoded_value){
    if (encoded_value[0] == 'i' && encoded_value[encoded_value.size()-1] == 'e'){
        std::string x = encoded_value.substr(1, encoded_value.length()-2);
        return json(std::stol(x));
    } else {
        throw std::runtime_error("Invalid encoded value: " + encoded_value);
    }
}

json decode_list(const std::string& encoded_value){
    if (encoded_value[0] == 'l' && encoded_value[encoded_value.size()-1] == 'e'){
        //decode list
        json list = json::array();

        std::string text = encoded_value.substr(1,encoded_value.length()-2);

        for (int i = 0; i < text.length()-1; )
            if (std::isdigit(text[i])){//decode string
                // printf("string found\n");
                int offset = 0;
                std::string count_of_chars = "";

                for (; text[i+offset] != ':';offset++)
                    count_of_chars+=text[i+offset];
                

                list.push_back(decode_string(text.substr(i,i+std::stoi(count_of_chars)-1)));
                i = i+std::stoi(count_of_chars)+offset+1;//end of string
            } 
            else if ( text[i] == 'i' && std::isdigit(text[i+1])){//decode int
                // printf("int found\n");
                std::string tmp = "";
                int count_of_chars = 0;

                for (; text[i+count_of_chars] != 'e'; count_of_chars++)
                    tmp+=text[i+count_of_chars];
                tmp+='e';

                list.push_back(decode_int(tmp));
                i = i+count_of_chars+1;
                

            }
        
        return list;
    }
    else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}

/*
    INT     "i52e"    --> 52                    integer <value> end
    STRING  "5:hello" --> "hello"               count_of_chars : <value>
    LIST    "li52e5:helloe" --> [52,"hello"]    list <values> end
*/
json decode_bencoded_value(const std::string& encoded_value) {
    
    if (std::isdigit(encoded_value[0])) {
        return decode_string(encoded_value);
    } 
    else if (encoded_value[0] == 'i' && encoded_value[encoded_value.size()-1] == 'e'){
        return decode_int(encoded_value);
    }
    else if (encoded_value[0] == 'l' && encoded_value[encoded_value.size()-1] == 'e'){
        return decode_list(encoded_value);
    }
    else {
        throw std::runtime_error("Unhandled encoded value: " + encoded_value);
    }
}