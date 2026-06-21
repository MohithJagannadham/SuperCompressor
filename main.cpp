#include "HuffmanCompressor.hpp"
#include "LZWCompressor.hpp"
#include "Exceptions.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <fstream>

// Helper function to reliably get file size using standard streams
double getFileSizeKB(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return 0.0;
    return file.tellg() / 1024.0;
}

void printHelp() {
    std::cout << "Usage: ./compressor [OPTIONS]\n"
              << "Options:\n"
              << "  --compress      Compress option\n"
              << "  --decompress    Decompress option\n"
              << "  --input <f>     Input file path\n"
              << "  --output <f>    Output file path\n"
              << "  --algo <a>      Algorithm: 'huffman' or 'lzw' (default: huffman)\n"
              << "  --stats         Print execution and payload metrics\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) { printHelp(); return 1; }

    bool isCompress = false, isDecompress = false, showStats = false;
    std::string input = "", output = "", algo = "huffman";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--compress") == 0) isCompress = true;
        else if (strcmp(argv[i], "--decompress") == 0) isDecompress = true;
        else if (strcmp(argv[i], "--stats") == 0) showStats = true;
        else if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) input = argv[++i];
        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) output = argv[++i];
        else if (strcmp(argv[i], "--algo") == 0 && i + 1 < argc) algo = argv[++i];
        else if (strcmp(argv[i], "--help") == 0) { printHelp(); return 0; }
    }

    if (input.empty() || output.empty() || (isCompress && isDecompress) || (!isCompress && !isDecompress)) {
        std::cerr << "Error: Invalid processing configurations or flags.\n";
        return 1;
    }

    try {
        auto start_time = std::chrono::high_resolution_clock::now();

        if (isCompress) {
            if (algo == "huffman") { HuffmanCompressor hc; hc.compress(input, output); }
            else { LZWCompressor lc; lc.compress(input, output); }
        } else {
            if (algo == "huffman") { HuffmanCompressor hc; hc.decompress(input, output); }
            else { LZWCompressor lc; lc.decompress(input, output); }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;

        std::cout << "Successfully " << (isCompress ? "compressed" : "decompressed") 
                  << " via " << algo << ".\n";

        // Print Hardware and IO Profiling Stats
        if (showStats) {
            double inSize = getFileSizeKB(input);
            double outSize = getFileSizeKB(output);
            
            if (inSize > 0 && outSize > 0) {
                double ratio = isCompress ? (outSize / inSize) * 100.0 : (inSize / outSize) * 100.0;

                std::cout << "\n--- Performance Metrics ---\n";
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Execution Time : " << duration.count() << " ms\n";
                std::cout << "Input Size     : " << inSize << " KB\n";
                std::cout << "Output Size    : " << outSize << " KB\n";
                if (isCompress) std::cout << "Space Ratio    : " << ratio << "% of original\n";
                std::cout << "---------------------------\n";
            } else {
                std::cerr << "\n[Warning] Could not read file sizes for metrics.\n";
            }
        }

    } catch (const FileIOError& e) {
        std::cerr << "[I/O FAILURE]: " << e.what() << "\n";
        return 1;
    } catch (const CompressionError& e) {
        std::cerr << "[CORRUPTION DETECTED]: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[FATAL EXCEPTION]: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}