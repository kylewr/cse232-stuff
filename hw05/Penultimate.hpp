#pragma once

#include <string>

using std::string;

class Penultimate {
public:
    Penultimate(string first) : first_(first) {}

    string push_back(string inp);

private:
    string first_;
};