#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

#include "lib/nlohmann/json.hpp"
#include "lib/sha1/sha1.hpp"
#include "lib/becodeUtil/BencodeUtil.cpp"

using json = nlohmann::json;
using namespace std;


json parse_torrent_file(const std::string& filename) {

    std::ifstream ifs(filename);
    if (ifs) {
        ifs.seekg(0, ifs.end);
        int length = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        char *buffer = new char[length];
        ifs.read(buffer, length);
        std::string torrent_data = buffer;

        int data_len;
        json data_res;
        std::tie(data_res,data_len) = decode_bencoded_value(torrent_data);
        return data_res;
    }
    throw std::runtime_error("Unable to find file: " + filename);

}

void GetTorrentInfo(const std::string &filename){
    json data = parse_torrent_file(filename);

    std::string enc_info = encode(data["info"]);
    SHA1 info_hash;
    info_hash.update(enc_info);


    std::cout << "---------------------------------------------"     << std::endl;
    std::cout << "Name:                " << data["info"]["name"]     << std::endl;
    std::cout << "Length:              " << data["info"]["length"]   << std::endl;
    std::cout << "INFO Hash(SHA-1):    " << info_hash.final()        << std::endl;
    std::cout << "Piece Length:        " << data["info"]["piece length"] << std::endl;
    std::cout << "---------------------------------------------"     << std::endl;
}

int main(int argc, char* argv[]) {

    // int len;
    // json res;

    // std::tie(res,len) = decode_bencoded_value("ld5:hello5:ghosted5:hello5:ghostee");
    // std::tie(res,len) = decode_bencoded_value("d3:ke1lll6:hi bro12:are u stupideee4:key2i13e4:key39:th stringe");
    GetTorrentInfo("true.torrent");

    // json res = parse_torrent_file("true.torrent");
    // cout << res["info"];
    // GetTorrentInfo("true.torrent");
    return 0;
}
