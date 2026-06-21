#include "LZWCompressor.hpp"
#include "Exceptions.hpp"
#include <iostream>
#include <fstream>
#include <vector>

void LZWCompressor::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);
    if (!in.is_open() || !out.is_open()) throw FileIOError("Failed to open files for LZW compression.");

    // Initialize root of Trie with base ASCII characters
    auto root = std::make_shared<TrieNode>();
    for (int i = 0; i < 256; i++) {
        root->children[static_cast<char>(i)] = std::make_shared<TrieNode>(i);
    }

    char ch;
    int dictSize = 256;
    auto current = root;

    // Traverse the Trie byte-by-byte
    while (in.get(ch)) {
        if (current->children.count(ch)) {
            current = current->children[ch];
        } else {
            out.write(reinterpret_cast<const char*>(&current->code), sizeof(current->code));
            current->children[ch] = std::make_shared<TrieNode>(dictSize++);
            current = root->children[ch]; // Reset to the new character's root path
        }
    }
    
    // Write out the final trailing code sequence
    if (current != root) {
        out.write(reinterpret_cast<const char*>(&current->code), sizeof(current->code));
    }
}

void LZWCompressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile, std::ios::binary);
    if (!in.is_open() || !out.is_open()) throw FileIOError("Failed to open files for LZW decompression.");

    // Vector replaces map: guarantees O(1) lookup and dense, cache-friendly memory allocation
    std::vector<std::string> dictionary(256);
    for (int i = 0; i < 256; i++) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    int previousCode, currentCode;
    if (in.read(reinterpret_cast<char*>(&previousCode), sizeof(previousCode))) {
        if (previousCode >= dictionary.size()) throw CompressionError("Corrupted starting token in LZW stream.");
        
        std::string s = dictionary[previousCode];
        out.write(s.data(), s.length());
        
        std::string entry;
        while (in.read(reinterpret_cast<char*>(&currentCode), sizeof(currentCode))) {
            if (currentCode < dictionary.size()) {
                entry = dictionary[currentCode];
            } else if (currentCode == dictionary.size()) {
                entry = dictionary[previousCode] + dictionary[previousCode][0];
            } else {
                throw CompressionError("Fatal Desync: Corrupted LZW data stream.");
            }
            
            out.write(entry.data(), entry.length());
            dictionary.push_back(dictionary[previousCode] + entry[0]);
            previousCode = currentCode;
        }
    }
}