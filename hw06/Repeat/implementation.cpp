#include "header.hpp"

int Repeat(const int& count) {
    return count * 2;
}

std::string Repeat(const char& ch) {
    return std::string(2, ch);
}

std::string Repeat(const std::string& str) {
    return str + " " + str;
}