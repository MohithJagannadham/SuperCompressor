#ifndef BIT_STREAM_HPP
#define BIT_STREAM_HPP

#include <fstream>

class BitWriter {
private:
    std::ofstream& out;
    unsigned char buffer;
    int bitCount;

public:
    explicit BitWriter(std::ofstream& outputStream);
    void writeBit(int bit);
    void flush();
};

class BitReader {
private:
    std::ifstream& in;
    unsigned char buffer;
    int bitCount;

public:
    explicit BitReader(std::ifstream& inputStream);
    int readBit(); // Returns 0 or 1, or -1 on End Of File (EOF)
};

#endif
