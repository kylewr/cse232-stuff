#include "ForgetfulVector.hpp"

void ForgetfulVector::push_back(int value)
{
    for (const int& v : seen_data_) {
        if (v == value) {
            data_.push_back(value);
            return;
        }
    }
    seen_data_.push_back(value);
}