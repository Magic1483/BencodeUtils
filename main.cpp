#include <iostream>
#include "src/lib/sha1/sha1.hpp"


void printHexRep(const unsigned char * test_sha) {

    std::cout << "CALLED HEX REP...PREPPING TO PRINT!\n";
    std::ostringstream os;
    os.fill('0');
    os << std::hex;
    for (const unsigned char * ptr = test_sha; ptr < test_sha + 20; ptr++) {

        os << std::setw(2) << (unsigned int) *ptr;
    }
    std::cout << os.str() << std::endl << std::endl;
}


int main() {

    using namespace std;

    ifstream myFile ("test.torrent", ifstream::binary);

    //Get file length
    myFile.seekg(0, myFile.end);
    int fileLength = myFile.tellg();
    myFile.seekg(0, myFile.beg);

    char buffer[fileLength];

    myFile.read(buffer, fileLength);
    cout << "File length == " << fileLength << endl;
    cout << buffer << endl << endl;

    SHA1 sha;
    sha.update(buffer);
    cout << sha.final();
    
    // unsigned char datSha[20];
    // SHA1((unsigned char *) buffer, fileLength, datSha);
    // printHexRep(datSha);

    myFile.close();

    return 0;
}