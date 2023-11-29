#include "HCTree.hpp"
#include "Helper.hpp"
#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    // Check command-line arguments
    if (argc != 3) {
        cerr << "Usage: ./compress <original_file> <compressed_file>" << endl;
        return 1;
    }

    // Open input file and count byte frequencies
    ifstream inFile(argv[1], ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open input file." << endl;
        return 1;
    }

    vector<int> freqs(256, 0);
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        freqs[byte]++;
    }

    // Build Huffman tree
    HCTree huffmanTree;
    huffmanTree.build(freqs);

    // Open output file
    FancyOutputStream outFile(argv[2]);

    // Write header (byte frequencies)
    for (int freq : freqs) {
        outFile.write_int(freq);
    }

    // Encode and write data
    inFile.clear();  // Clear EOF flag
    inFile.seekg(0); // Seek back to the beginning of the file
    while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        huffmanTree.encode(byte, outFile);
    }

    // Close files
    inFile.close();
    return 0;
}

