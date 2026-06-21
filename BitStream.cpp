#include "BitStream.hpp"

BitWriter::BitWriter(std::ofstream& outputStream) 
    : out(outputStream), buffer(0), bitCount(0) {}

void BitWriter::writeBit(int bit) {
    buffer = (buffer << 1) | (bit & 1);
    bitCount++;
    if (bitCount == 8) {
        out.write(reinterpret_cast<char*>(&buffer), 1);
        buffer = 0;
        bitCount = 0;
    }
}

void BitWriter::flush() {
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        out.write(reinterpret_cast<char*>(&buffer), 1);
        buffer = 0;
        bitCount = 0;
    }
}

BitReader::BitReader(std::ifstream& inputStream) 
    : in(inputStream), buffer(0), bitCount(0) {}

int BitReader::readBit() {
    if (bitCount == 0) {
        char ch;
        if (!in.get(ch)) return -1; // EOF reached
        buffer = static_cast<unsigned char>(ch);
        bitCount = 8;
    }
    int bit = (buffer >> (bitCount - 1)) & 1;
    bitCount--;
    return bit;
}
