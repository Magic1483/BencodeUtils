#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cassert>

#include "lib/nlohmann/json.hpp"
#include "lib/becodeUtil/BencodeUtil.hpp"

using json = nlohmann::json;
using namespace std;

void test(){
    json j = json::array({1,2,"hello",52,"yamite kudasai",458});
    std::string encoded = encode_list(j);

    int test_int = 52;
    std::string enc_test_int = "i52e";


    std::string test_str = "yamite kudasai";
    std::string enc_test_str = "14:yamite kudasai";

    // std::cout << (decode_bencoded_value(encoded) == j) << " " << encoded <<std::endl;


    cout << "-------------------------------" << endl;
    cout << "       | BENCODE TEXT |" << endl;
    cout << "-------------------------------" << endl;

    assert((enc_test_int == encode_int(test_int) ));
    cout << "|    Int encode passed!    |"          << endl;

    assert((test_int == decode_bencoded_value(enc_test_int) ));
    cout << "|    Int decode passed!    |"          << endl;
    cout << "\n---------------------------------\n" << endl;

    assert((enc_test_str == encode_string(test_str) ));
    cout << "|    String encode passed! |"          << endl;

    assert((test_str == decode_bencoded_value(enc_test_str) ));
    cout << "|    String decode passed! |" << endl;
    cout << "\n---------------------------------\n" << endl;


    assert((encoded == "li1ei2e5:helloi52e14:yamite kudasaii458ee"));
    cout << "|    Array encode passed!  |"          << endl;

    assert((decode_bencoded_value(encoded) == j));
    cout << "|    Array decode passed!  |"          << endl;

    cout << "-------------------------------"       << endl;
    cout << "       | ALL PASSED |"                 << endl;
    cout << "-------------------------------"       << endl;
}



int main(int argc, char* argv[]) {
    test();
    
    // cout << encode_list(json::array({1,2,"word is suck"}));

    return 0;
}
