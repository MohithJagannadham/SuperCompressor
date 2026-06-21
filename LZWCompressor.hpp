#ifndef LZW_COMPRESSOR_HPP
#define LZW_COMPRESSOR_HPP

#include <string>
#include <unordered_map>
#include <memory>

// Advanced Space-Optimized Trie Data Structure
struct TrieNode {
    int code;
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    
    explicit TrieNode(int c = -1) : code(c) {}
};

class LZWCompressor {
public:
    void compress(const std::string& inputFile, const std::string& outputFile);
    void decompress(const std::string& inputFile, const std::string& outputFile);
};

#endif