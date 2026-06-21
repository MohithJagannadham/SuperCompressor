# SuperCompressor

A high-performance, modular command-line file compression utility written in C++17. It supports both **Huffman Coding** and **Lempel-Ziv-Welch (LZW)** algorithms, featuring a decoupled bit-stream I/O layer, advanced tree-based memory optimizations, custom error handling, and hardware execution profiling.

## Features

* **Multi-Algorithm Engine:** Switch seamlessly between Huffman and LZW compression algorithms on the fly via clear command-line flags.
* **Space-Optimized LZW Trie:** Replaces standard hash maps with a custom-engineered Trie (Prefix Tree) for dictionary generation during compression, eliminating string allocation overhead and optimizing memory footprint.
* **High-Performance Decompression:** Utilizes a dense, continuous `std::vector` cache for LZW decompression, guaranteeing strict $O(1)$ constant-time dictionary lookups.
* **Deterministic Huffman Tree Generation:** Enforces strict frequency sorting and alphanumeric character tie-breakers within the internal min-heap, ensuring 100% stable tree cloning and lossless data parity.
* **Hardware & I/O Performance Profiling:** Features an optional `--stats` execution flag leveraging high-resolution `<chrono>` tracking and standard stream boundaries to calculate exact runtimes (ms), input/output payloads (KB), and file space ratios.
* **Domain-Specific Exception Safety:** Implements strongly-typed custom exception structures (`FileIOError`, `CompressionError`) to handle corrupted files, missing streams, and invalid decoding tokens safely.
* **Zero-Dependency Native Test Harness:** Built with a standalone, highly portable C++ testing suite using native assertions (`cassert`), ensuring seamless compilation across older or non-POSIX toolchains without external libraries.
* **Modern C++ Architecture:** Fully compliant with C++17 RAII principles, utilizing smart pointers (`std::shared_ptr`) for all dynamic tree node life cycles to guarantee zero memory leaks.

## Project Structure

```text
SuperCompressor/
├── CMakeLists.txt         # Cross-platform build automation file
├── BitStream.hpp/cpp      # Low-level bit-shifting abstractions & byte-alignment
├── HuffmanCompressor.* # Deterministic Huffman coding & tree traversal modules
├── LZWCompressor.hpp/cpp  # Space-optimized LZW compression via Custom Trie structures
├── Exceptions.hpp         # Custom domain-specific error handling hierarchies
├── main.cpp               # Application router, CLI orchestration, and profiling engine
└── tests.cpp              # Standalone native assertion testing suite
Prerequisites
A C++17 compatible compiler (GCC, MinGW-w64, Clang, or MSVC)

CMake (v3.14 or higher)

Compilation Process
This project uses CMake to abstract and automate the compilation process across different developer environments.

1. Generate the Build Environment
Open your terminal in the root directory of the project. If you are developing on Windows utilizing a standard MinGW/GCC toolchain, explicitly invoke the MinGW generator:

Bash
cmake -G "MinGW Makefiles" -B build
(For Linux, macOS, or environments using Microsoft Visual Studio Build Tools, simply run: cmake -B build)

2. Compile the Executables
Instruct CMake to invoke the underlying compiler and build both the main compression utility and the automated test suite:

Bash
cmake --build build
Upon a successful 100% compilation, this generates two runnable binaries inside the ./build folder: compressor.exe and run_tests.exe.

Detailed Usage
1. Compiling with Performance Metrics
Run the tool with the --compress or --decompress action flags, specify the target paths, choose your algorithm (huffman or lzw), and pass the --stats flag to profile the execution metrics:

Bash
./build/compressor --compress --input massive_payload.txt --output massive.bin --algo lzw --stats
Example Output:

Plaintext
Successfully compressed via lzw.

--- Performance Metrics ---
Execution Time : 159.26 ms
Input Size     : 1201.17 KB
Output Size    : 89.92 KB
Space Ratio    : 7.49% of original
---------------------------
2. Decompressing Files
To restore a file back to its original loss-free state, swap to the --decompress flag:

Bash
./build/compressor --decompress --input massive.bin --output restored.txt --algo lzw
Running the Automated Test Suite
To verify the functional correctness of the bit-stream mechanics, Huffman encoding parameters, and LZW dictionary traversal, run the standalone native test binary directly:

Bash
./build/run_tests
Expected Output:

Plaintext
--- Running SuperCompressor Native Test Suite ---
[PASS] BitStream Write/Read Parity
[PASS] Huffman Mathematical Parity
[PASS] LZW Mathematical Parity
-------------------------------------------------
SUCCESS: All core logic modules verified perfectly.