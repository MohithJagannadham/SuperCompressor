#ifndef HUFFMAN_COMPRESSOR_HPP
#define HUFFMAN_COMPRESSOR_HPP

#include <string>
#include <map>
#include <memory>

struct HuffmanNode {
    char ch;
    int freq;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(char character, int frequency);
};

struct CompareNodes {
    bool operator()(const std::shared_ptr<HuffmanNode>& n1, const std::shared_ptr<HuffmanNode>& n2);
};

class HuffmanCompressor {
private:
    // Updated to use std::map
    void generateCodes(const std::shared_ptr<HuffmanNode>& root, const std::string& str, std::map<char, std::string>& huffmanCode);

public:
    void compress(const std::string& inputFile, const std::string& outputFile);
    void decompress(const std::string& inputFile, const std::string& outputFile);
};

#endif