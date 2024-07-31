#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "lib/nlohmann/json.hpp"
#include "lib/sha1/sha1.hpp"

#include "Torrent.cpp"
// #include "lib/utils.cpp"

using json = nlohmann::json;
using namespace std;









// void GetTorrentInfo(const std::string &filename){
//     json data = parse_torrent_file(filename);

//     std::string enc_info = encode(data["info"]);
//     SHA1 info_hash;
//     info_hash.update(enc_info);

//     std::cout << "---------------------------------------------"     << std::endl;
//     std::cout << "Name:                " << data["info"]["name"]     << std::endl;
//     std::cout << "Piece Length:        " << data["info"]["piece length"]   << std::endl;
//     std::cout << "INFO Hash(SHA-1):    " << info_hash.final()        << std::endl;
//     std::cout << "---------------------------------------------"     << std::endl;

//     std::cout << "Length        " << data["info"]["length"]  << std::endl;
//     std::cout << "Pieces Hash:           "  << std::endl;
    
//     std::vector<uint8_t> vec = data["info"]["pieces"];
//     int i = 0;
//     while (i < data["info"]["pieces"].size())
//     {
//         std::vector<uint8_t> tmp;
//         std::stringstream ss;
//         tmp.assign(vec.begin()+i,vec.begin()+i+20);
//         for (int num : tmp) {
//             ss << std::hex << std::setw(2) << std::setfill('0') << num; // write data to stringstream
//         } 
//         std::cout << ss.str() << std::endl;
//         i+=20;
//     }
// }

std::string readFile(std::string filename){
    std::ifstream t(filename);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    return buffer;

}

std::string bparse_string(std::string s){
    int colon_ind = s.find(":");
    std::string str_number = s.substr(0,colon_ind);
    cout << str_number << "\n";

    return s.substr(str_number.size()+1,stoll(str_number));
}


std::string bencode_string(std::string s){
    return s.size()+":"+s;
}

std::string GetRawPieces(std::string filename){
    std::string res = readFile(filename);

    std::string target = "pieces";
    size_t part1  = res.find(target);
    std::string pieces  = res.substr(part1+target.size());

    json j;
    j = bparse_string(pieces);
    return j;
}

int main(int argc, char* argv[]) {

    Torrent t;
    t.ReadFile("sample.torrent");
    

    std::string bdata = encode(t.TorrentData["info"]);
    // cout << bdata;


    std::string target = "pieces0:";

    std::string part1 = bdata.substr(0,bdata.find(target)+target.size()-2);
    std::string part2 = bdata.substr(bdata.find(target)+target.size());



    std::stringstream ss;
    ss <<  part1 << bencode_string(GetRawPieces("sample.torrent")) << part2;
    std::string res = ss.str();

    const char* binaryInfoDictionary = res.c_str();
    int length = res.length();
    unsigned char* binaryData = new unsigned char[length];
    for (int i = 0; i < length; i++) {
        binaryData[i] = (unsigned char)binaryInfoDictionary[i];
    }

    SHA1 sha;
    sha.update(binaryInfoDictionary);
    cout << sha.final();
    

    return 0;
}



// d8:announce55:http://bittorrent-test-tracker.codecrafters.io/announce10:created by13:mktorrent 1.14:infod6:lengthi92063e4:name10:sample.txt12:piece lengthi32768e6:pieces60:�v�z*����kg&���-n"u��vfVsn���R��5��z����	r'�����ee
// ôz☻‼▀↓é╝ìce55:htr'¡₧ÉÜ╠↨eerrent-test-tracker.codecrafters.io/announce10:created by13:mktorrent 1.14:infod6:lengthi92063e4:name10:sample.txt12:piece lengthi32768e6:pieces60:Φv÷z*êåΦ≤k‼g&├☼óù♥☻-n"uµ♦ávfVsnü ►╡R♦¡ì5≡