#include <string>
#include <iostream>
#include <fstream>
#include <vector>


/*
https://gist.github.com/litefeel/1197e5c24eb9ec93d771
*/



std::string readFileStr(const std::string& filepath){
    if (std::ifstream is{filepath,std::ios::binary | std::ios::ate}){
        auto size = is.tellg();//get size
        std::string str(size,'\0');//string of '\0' with the length is 'size'. std::string
        is.seekg(0);//change read position
        
        if (is.read(&str[0],size)){
            return str;
        }           
        throw std::runtime_error("Error reading file");
    }
}