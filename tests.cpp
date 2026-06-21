#include "HuffmanCompressor.hpp"
#include "LZWCompressor.hpp"
#include "BitStream.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <cassert>

std::string readAllText(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

void testBitStream() {
    std::string tempFile = "bit_test.bin";
    {
        std::ofstream out(tempFile, std::ios::binary);
        BitWriter writer(out);
        writer.writeBit(1); writer.writeBit(1);
        writer.writeBit(0); writer.writeBit(0);
        writer.writeBit(1); writer.writeBit(0);
        writer.writeBit(1); writer.writeBit(0);
        writer.flush(); 
    }
    {
        std::ifstream in(tempFile, std::ios::binary);
        BitReader reader(in);
        assert(reader.readBit() == 1); assert(reader.readBit() == 1);
        assert(reader.readBit() == 0); assert(reader.readBit() == 0);
        assert(reader.readBit() == 1); assert(reader.readBit() == 0);
        assert(reader.readBit() == 1); assert(reader.readBit() == 0);
        assert(reader.readBit() == -1); 
    }
    std::remove(tempFile.c_str()); 
    std::cout << "[PASS] BitStream Write/Read Parity\n";
}

void testHuffman() {
    std::string testInputFile = "test_raw.txt";
    std::string testCompressedFile = "test_out_h.bin";
    std::string testRestoredFile = "test_restored_h.txt";
    std::string samplePayload = "BitStream isolation verification loop! The quick brown fox jumps over 12345.";

    std::ofstream out(testInputFile, std::ios::binary);
    out << samplePayload;
    out.close();

    HuffmanCompressor hc;
    hc.compress(testInputFile, testCompressedFile);
    hc.decompress(testCompressedFile, testRestoredFile);

    assert(samplePayload == readAllText(testRestoredFile));

    std::remove(testInputFile.c_str());
    std::remove(testCompressedFile.c_str());
    std::remove(testRestoredFile.c_str());
    std::cout << "[PASS] Huffman Mathematical Parity\n";
}

void testLZW() {
    std::string testInputFile = "test_raw.txt";
    std::string testCompressedFile = "test_out_l.bin";
    std::string testRestoredFile = "test_restored_l.txt";
    std::string samplePayload = "BitStream isolation verification loop! The quick brown fox jumps over 12345.";

    std::ofstream out(testInputFile, std::ios::binary);
    out << samplePayload;
    out.close();

    LZWCompressor lc;
    lc.compress(testInputFile, testCompressedFile);
    lc.decompress(testCompressedFile, testRestoredFile);

    assert(samplePayload == readAllText(testRestoredFile));

    std::remove(testInputFile.c_str());
    std::remove(testCompressedFile.c_str());
    std::remove(testRestoredFile.c_str());
    std::cout << "[PASS] LZW Mathematical Parity\n";
}

int main() {
    std::cout << "--- Running SuperCompressor Native Test Suite ---\n";
    testBitStream();
    testHuffman();
    testLZW();
    std::cout << "-------------------------------------------------\n";
    std::cout << "SUCCESS: All core logic modules verified perfectly.\n";
    return 0;
}