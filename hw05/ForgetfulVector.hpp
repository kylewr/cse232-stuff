#pragma once

#include <vector>

class ForgetfulVector {
public:
    ForgetfulVector() : data_() {}

    void push_back(int value);

    int at(size_t index) const {
        return data_.at(index);
    }

    size_t size() const {
        return data_.size();
    }

private:
    std::vector<int> data_;
    std::vector<int> seen_data_;
};