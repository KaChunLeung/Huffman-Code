#include "HCTree.hpp"
#include "Helper.hpp"
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    // Check command-line arguments
    if (argc != 3) {
        cerr << "Usage: ./uncompress <compressed_file> <uncompressed_file>" << endl;
        return 1;
    }

    // Open compressed file
    FancyInputStream inFile(argv[1]);
    // The is_open() check is removed

    // Read header to get frequencies
    vector<int> freqs(256, 0);
    for (int i = 0; i < 256; ++i) {
        freqs[i] = inFile.read_int();
    }

    // Build Huffman tree
    HCTree huffmanTree;
    huffmanTree.build(freqs);

    // Open output file
    ofstream outFile(argv[2], ios::binary);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot open output file." << endl;
        return 1;
    }

    // Decode and write data
    unsigned char decodedByte;
    while (true) {
        try {
            decodedByte = huffmanTree.decode(inFile);
        } catch (const std::exception& e) {
            // Break the loop if an EOF or similar error is caught
            break;
        }
        outFile.put(decodedByte);
    }

    // Close files
    outFile.close();
    return 0;
}

