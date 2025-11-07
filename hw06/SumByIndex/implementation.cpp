#include "header.hpp"

#include <vector>

std::vector<int> SumByIndex(std::vector<std::vector<int>> const& matrix) {
    std::vector<int> result;

    if (matrix.empty()) {
        return result;
    }

    for (const int& value : matrix.at(0)) {
        result.push_back(value);
    }

    for (size_t i = 1; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix.at(i).size(); j++) {
            if (j < result.size()) {
                result.at(j) += matrix.at(i).at(j);
            } else {
                result.push_back(matrix.at(i).at(j));
            }
        }
    }

    return result;
}

