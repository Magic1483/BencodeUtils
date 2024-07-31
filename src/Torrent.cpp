#include <iostream>
#include <string>
#include <fstream>

#include "lib/utils.cpp"
// #include "lib/HTTPRequests/HTTPRequest.hpp"
#include "lib/nlohmann/json.hpp"
#include "lib/becodeUtil/BencodeUtil.cpp"
#include "lib/sha1/sha1.hpp"


using json = nlohmann::json;

/*
https://github.com/elnormous/HTTPRequest
https://www.geeksforgeeks.org/cpp-class-methods/
*/


class Torrent
{
public:
    json TorrentData;

    std::string url;
    std::string info_hash;
    std::string name;

    int client_port;
    
    

    // void GetPeers();
    void ReadFile(std::string filename);
    void GetInfo();
    
};

void Torrent::ReadFile(std::string filename){
    std::string torrent_data = readFileStr(filename);
    int data_len;
    json data_res;

    std::tie(data_res,data_len) = decode_bencoded_value(torrent_data);
    
    TorrentData = data_res;
    url = TorrentData["announce"];
    name = TorrentData["info"]["name"];

    TorrentData["info"]["pieces"] = "";
    // std::cout << TorrentData["info"];
    // SHA1 hash;
    // hash.update(encode(TorrentData["info"]));
    // info_hash = hash.final();

}

void Torrent::GetInfo(){
    std::cout << "---------------------------------------------"     << std::endl;
    std::cout << "Announce url         " << url                      << std::endl;
    std::cout << "Name:                " << name                     << std::endl;
    std::cout << "INFO Hash(SHA-1):    " << info_hash                << std::endl;
    std::cout << "---------------------------------------------"     << std::endl;
}

// void Torrent::GetPeers(){
//         try
//         {
//             http::Request request{url};
            

//             const auto response = request.send("GET");
//             std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; // print the result
//         }
//         catch (const std::exception& e)
//         {
//             std::cerr << "Request failed, error: " << e.what() << '\n';
//         }
// }


// int main(int argc, char const *argv[])
// {
    
//     Torrent t;
//     t.ReadFile("sample.torrent");

//     t.GetInfo();
//     // t.GetPeers();
//     return 0;
// }
