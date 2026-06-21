#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class FileIOError : public std::runtime_error {
public:
    explicit FileIOError(const std::string& message) : std::runtime_error(message) {}
};

class CompressionError : public std::runtime_error {
public:
    explicit CompressionError(const std::string& message) : std::runtime_error(message) {}
};

#endif