#include "HuffmanCompressor.hpp"
#include "BitStream.hpp"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>

const size_t CHUNK_SIZE = 4096;

HuffmanNode::HuffmanNode(char character, int frequency) 
    : ch(character), freq(frequency), left(nullptr), right(nullptr) {}

bool CompareNodes::operator()(const std::shared_ptr<HuffmanNode>& n1, const std::shared_ptr<HuffmanNode>& n2) {
    // Tie-breaker guarantees stable, deterministic sorting
    if (n1->freq == n2->freq) {
        return n1->ch > n2->ch; 
    }
    return n1->freq > n2->freq;
}

void HuffmanCompressor::generateCodes(const std::shared_ptr<HuffmanNode>& root, const std::string& str, std::map<char, std::string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }
    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

void HuffmanCompressor::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("Cannot open input file.");

    // Changed to map for determinism
    std::map<char, int> freqMap; 
    long long totalChars = 0;
    std::vector<char> buffer(CHUNK_SIZE);
    
    while (in.read(buffer.data(), buffer.size()) || in.gcount() > 0) {
        for (std::streamsize i = 0; i < in.gcount(); ++i) {
            freqMap[buffer[i]]++;
            totalChars++;
        }
    }

    if (freqMap.empty()) return;

    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, CompareNodes> minHeap;
    for (const auto& pair : freqMap) {
        minHeap.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        auto left = minHeap.top(); minHeap.pop();
        auto right = minHeap.top(); minHeap.pop();
        auto top = std::make_shared<HuffmanNode>('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }

    auto root = minHeap.top();
    std::map<char, std::string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    std::ofstream out(outputFile, std::ios::binary);
    int mapSize = freqMap.size();
    out.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    for (const auto& pair : freqMap) {
        out.write(&pair.first, sizeof(pair.first));
        out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }
    
    out.write(reinterpret_cast<const char*>(&totalChars), sizeof(totalChars));

    in.clear();
    in.seekg(0, std::ios::beg);

    BitWriter bitWriter(out);
    while (in.read(buffer.data(), buffer.size()) || in.gcount() > 0) {
        for (std::streamsize i = 0; i < in.gcount(); ++i) {
            const std::string& code = huffmanCode[buffer[i]];
            for (char bit : code) {
                bitWriter.writeBit(bit - '0');
            }
        }
    }
    bitWriter.flush();
}

void HuffmanCompressor::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("Cannot open compressed file.");

    int mapSize;
    if (!in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize))) return;

    // Changed to map for determinism
    std::map<char, int> freqMap; 
    for (int i = 0; i < mapSize; ++i) {
        char ch; int freq;
        in.read(&ch, sizeof(ch));
        in.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        freqMap[ch] = freq;
    }

    long long totalChars;
    in.read(reinterpret_cast<char*>(&totalChars), sizeof(totalChars));

    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, CompareNodes> minHeap;
    for (const auto& pair : freqMap) {
        minHeap.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }
    while (minHeap.size() > 1) {
        auto left = minHeap.top(); minHeap.pop();
        auto right = minHeap.top(); minHeap.pop();
        auto top = std::make_shared<HuffmanNode>('\0', left->freq + right->freq);
        top->left = left; top->right = right;
        minHeap.push(top);
    }

    auto root = minHeap.top();
    std::ofstream out(outputFile, std::ios::binary);
    std::shared_ptr<HuffmanNode> current = root;
    
    BitReader bitReader(in);
    while (totalChars > 0) {
        int bit = bitReader.readBit();
        if (bit == -1) break;
        
        current = (bit == 0) ? current->left : current->right;
        
        if (!current->left && !current->right) {
            out.put(current->ch);
            current = root;
            totalChars--;
        }
    }
}